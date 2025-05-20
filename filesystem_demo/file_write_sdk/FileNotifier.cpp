#include "FileNotifier.hpp"

#include <cerrno>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

FileNotifier::FileNotifier(const std::string& path, uint32_t mask,
                           Callback callback)
    : path_(path),
      mask_(mask),
      callback_(std::move(callback)),
      running_(false) {
  std::cout << __func__ << " begin. watch path: " << path_ << std::endl;
  inotify_fd_ = inotify_init1(IN_NONBLOCK);
  if (inotify_fd_ == -1) {
    throw std::runtime_error("inotify_init1 failed: " +
                             std::string(strerror(errno)));
  }

  watch_fd_ = inotify_add_watch(inotify_fd_, path_.c_str(), mask_);
  if (watch_fd_ == -1) {
    close(inotify_fd_);
    throw std::runtime_error("inotify_add_watch failed: " +
                             std::string(strerror(errno)));
  }
}

FileNotifier::~FileNotifier() {
  std::cout << __func__ << " begin\n";
  stop();
  if (inotify_fd_ != -1) {
    inotify_rm_watch(inotify_fd_, watch_fd_);
    close(inotify_fd_);
  }
}

void FileNotifier::start() {
  if (running_) {
    return;
  }
  running_ = true;
  monitor_thread_ = std::thread(&FileNotifier::monitorLoop, this);
}

void FileNotifier::stop() {
  std::cout << "Stopping file notifier...\n";
  running_ = false;
  if (monitor_thread_.joinable()) {
    monitor_thread_.join();
  }
}

// Helper method to convert event mask to string representation
std::string getEventTypeString(uint32_t mask) {
  std::string result;
  if (mask & IN_ACCESS) result += "IN_ACCESS ";
  if (mask & IN_MODIFY) result += "IN_MODIFY ";
  if (mask & IN_ATTRIB) result += "IN_ATTRIB ";
  if (mask & IN_CLOSE_WRITE) result += "IN_CLOSE_WRITE ";
  if (mask & IN_CLOSE_NOWRITE) result += "IN_CLOSE_NOWRITE ";
  if (mask & IN_OPEN) result += "IN_OPEN ";
  if (mask & IN_MOVED_FROM) result += "IN_MOVED_FROM ";
  if (mask & IN_MOVED_TO) result += "IN_MOVED_TO ";
  if (mask & IN_CREATE) result += "IN_CREATE ";
  if (mask & IN_DELETE) result += "IN_DELETE ";
  if (mask & IN_DELETE_SELF) result += "IN_DELETE_SELF ";
  if (mask & IN_MOVE_SELF) result += "IN_MOVE_SELF ";
  return result.empty() ? "UNKNOWN" : result;
}

void FileNotifier::monitorLoop() {
  constexpr size_t eventSize = sizeof(inotify_event);
  constexpr size_t bufferSize = 1024 * (eventSize + 16);

  while (running_) {
    char buffer[bufferSize];
    ssize_t bytes_read = read(inotify_fd_, buffer, bufferSize);

    // std::cout << "Read inotify events. bytesRead: " << bytesRead
    //           << ", errno: " << errno << std::endl;

    if (bytes_read == -1) {
      // std::cout << "Error reading inotify events. err: " << strerror(errno)
      //           << " (errno: " << errno << ")" << std::endl;
      if (errno == EBADF || errno == EFAULT || errno == EINVAL ||
          errno == ENOMEM || errno == EINTR) {
        // Bad file descriptor, bad address, or invalid argument
        std::cerr << "Critical error reading inotify events: "
                  << strerror(errno) << " (errno: " << errno << ")"
                  << std::endl;
        break;
      }
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // Non-blocking read with no data available, this is normal
        // std::cout << "No data available for reading inotify events.\n";
      } else {
        // Other unexpected errors
        std::cerr << "Unexpected error reading inotify events: "
                  << strerror(errno) << " (errno: " << errno << ")"
                  << std::endl;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      continue;
    }

    for (char* ptr = buffer; ptr < buffer + bytes_read;) {
      inotify_event* event = reinterpret_cast<inotify_event*>(ptr);

      if (event->mask & IN_Q_OVERFLOW) {
        std::cerr << "Warning: inotify event queue overflowed" << std::endl;
        ptr += eventSize + event->len;
        continue;
      }

      if (event->mask & mask_) {
        std::string filename = path_;
        if (event->len > 0) {
          filename += "/" + std::string(event->name);
        }
        std::cout << "File event captured. path: " << filename
                  << ", event type: " << getEventTypeString(event->mask)
                  << std::endl;
        callback_(filename);
      }

      ptr += eventSize + event->len;
    }
  }
}
