#include <limits.h>
#include <sys/inotify.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

// 显示 inotify 事件的辅助函数
void display_event(const struct inotify_event* event) {
  std::cout << "Event detected on " << event->wd << ": ";

  if (event->mask & IN_ACCESS) std::cout << "File accessed (read). ";
  if (event->mask & IN_MODIFY) std::cout << "File modified. ";
  if (event->mask & IN_ATTRIB) std::cout << "Metadata changed. ";
  if (event->mask & IN_CLOSE_WRITE)
    std::cout << "File opened for writing was closed. ";
  if (event->mask & IN_CLOSE_NOWRITE)
    std::cout << "File opened read-only was closed. ";
  if (event->mask & IN_OPEN) std::cout << "File opened. ";
  if (event->mask & IN_DELETE_SELF) std::cout << "File deleted. ";
  if (event->mask & IN_MOVE_SELF) std::cout << "File moved/renamed. ";

  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <file-to-watch>" << std::endl;
    return 1;
  }

  const std::string file_path = argv[1];

  // 初始化 inotify
  int inotify_fd = inotify_init();
  if (inotify_fd == -1) {
    perror("inotify_init");
    return 1;
  }

  // 添加文件监视
  int watch_desc = inotify_add_watch(
      inotify_fd, file_path.c_str(),
      IN_MODIFY | IN_ACCESS | IN_ATTRIB | IN_CLOSE_WRITE | IN_CLOSE_NOWRITE |
          IN_OPEN | IN_DELETE_SELF | IN_MOVE_SELF);

  if (watch_desc == -1) {
    perror("inotify_add_watch");
    close(inotify_fd);
    return 1;
  }

  std::cout << "Watching file: " << file_path << std::endl;
  std::cout << "Press Ctrl+C to exit..." << std::endl;

  // 事件缓冲区
  const size_t BUF_LEN = 1024 * (sizeof(struct inotify_event) + NAME_MAX + 1);
  char buffer[BUF_LEN];

  // 主事件循环
  while (true) {
    ssize_t num_read = read(inotify_fd, buffer, BUF_LEN);

    if (num_read == -1) {
      perror("read");
      break;
    }

    // 处理所有接收到的事件
    for (char* p = buffer; p < buffer + num_read;) {
      struct inotify_event* event = reinterpret_cast<struct inotify_event*>(p);
      display_event(event);

      // 如果文件被删除或移动，退出程序
      if (event->mask & (IN_DELETE_SELF | IN_MOVE_SELF)) {
        std::cout << "File was deleted or moved. Exiting." << std::endl;
        close(inotify_fd);
        return 0;
      }

      p += sizeof(struct inotify_event) + event->len;
    }
  }

  // 清理
  inotify_rm_watch(inotify_fd, watch_desc);
  close(inotify_fd);
  return 0;
}
