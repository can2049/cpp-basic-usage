#include "FileNotifier.hpp"

#include <chrono>
#include <iostream>

FileNotifier::FileNotifier(const std::string& directory, Callback callback)
    : directory_(directory), callback_(std::move(callback)), running_(false) {
  inotifyFd_ = inotify_init1(IN_NONBLOCK);
  if (inotifyFd_ == -1) {
    throw std::runtime_error("inotify_init1 failed: " +
                             std::string(strerror(errno)));
  }

  // 监控文件关闭写入和移动完成事件
  const uint32_t mask = IN_CLOSE_WRITE | IN_MOVED_TO | IN_Q_OVERFLOW;
  watchDescriptor_ = inotify_add_watch(inotifyFd_, directory_.c_str(), mask);
  if (watchDescriptor_ == -1) {
    close(inotifyFd_);
    throw std::runtime_error("inotify_add_watch failed: " +
                             std::string(strerror(errno)));
  }
}

FileNotifier::~FileNotifier() {
  stop();
  if (inotifyFd_ != -1) {
    inotify_rm_watch(inotifyFd_, watchDescriptor_);
    close(inotifyFd_);
  }
}

void FileNotifier::start() {
  if (running_) {
    return;
  }
  running_ = true;
  monitorThread_ = std::thread(&FileNotifier::monitorLoop, this);
}

void FileNotifier::stop() {
  running_ = false;
  if (monitorThread_.joinable()) {
    monitorThread_.join();
  }
}

void FileNotifier::monitorLoop() {
  constexpr size_t eventSize = sizeof(inotify_event);
  constexpr size_t bufferSize = 1024 * (eventSize + 16);

  while (running_) {
    char buffer[bufferSize];
    ssize_t bytesRead = read(inotifyFd_, buffer, bufferSize);

    if (bytesRead == -1) {
      if (errno == EAGAIN) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        continue;
      }
      break;
    }

    for (char* ptr = buffer; ptr < buffer + bytesRead;) {
      inotify_event* event = reinterpret_cast<inotify_event*>(ptr);

      if (event->mask & IN_Q_OVERFLOW) {
        std::cerr << "Warning: inotify event queue overflowed" << std::endl;
        ptr += eventSize + event->len;
        continue;
      }

      if ((event->mask & (IN_CLOSE_WRITE | IN_MOVED_TO)) && event->len > 0) {
        std::string filename = directory_ + "/" + event->name;
        std::cout << "File event. path: " << filename << std::endl;
        callback_(filename);
      }

      ptr += eventSize + event->len;
    }
  }
}
