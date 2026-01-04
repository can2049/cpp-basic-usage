#define _GNU_SOURCE
#include <dlfcn.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
namespace {

constexpr char g_log_file[] = "/tmp/io_monitor.log";

struct IOStat {
  int64_t read_cnt = 0;
  int64_t write_cnt = 0;
  int64_t read_bytes = 0;
  int64_t write_bytes = 0;
};

std::mutex g_mutex;
std::unordered_map<int, std::string> g_fd_to_path;
std::unordered_map<std::string, IOStat> g_stats;
std::atomic<bool> g_running = false;

/* 原始函数指针 */
int (*real_open)(const char*, int, ...) = nullptr;
ssize_t (*real_read)(int, void*, size_t) = nullptr;
ssize_t (*real_write)(int, const void*, size_t) = nullptr;
int (*real_close)(int) = nullptr;

[[maybe_unused]] std::string resolve_path(int fd) {
  char link_path[64];
  char real_path[PATH_MAX];
  snprintf(link_path, std::size(link_path), "/proc/self/fd/%d", fd);
  ssize_t len = readlink(link_path, real_path, sizeof(real_path) - 1);
  if (len > 0) {
    real_path[len] = '\0';
    return std::string(real_path);
  }
  return "unknown";
}

void logger_thread() {
  while (g_running.load()) {
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::unordered_map<std::string, IOStat> snapshot;
    {
      std::lock_guard<std::mutex> lk(g_mutex);
      snapshot = g_stats;
    }

    std::ofstream ofs(g_log_file, std::ios::app);
    ofs << "===== IO STAT =====\n";

    // 复制到向量
    std::vector<std::pair<std::string, IOStat>> entries(snapshot.begin(),
                                                        snapshot.end());
    std::cout << " entrys size: " << entries.size() << "\n";

    // 按 read_cnt 降序排序
    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
      return a.second.read_cnt > b.second.read_cnt;
    });
    ofs << "--- Top 10 by read count ---\n";
    ofs << "read_cnt\t read_bytes\t write_cnt\t write_bytes\t path\n";
    for (size_t i = 0; i < std::min(entries.size(), size_t(10)); ++i) {
      const auto& [path, stat] = entries[i];
      ofs << stat.read_cnt << "\t" << stat.read_bytes << "\t" << stat.write_cnt
          << "\t" << stat.write_bytes << "\t" << path << "\n";
    }

    // 按 write_cnt 降序排序
    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
      return a.second.write_cnt > b.second.write_cnt;
    });
    ofs << "--- Top 10 by write count ---\n";
    ofs << "read_cnt\t read_bytes\t write_cnt\t write_bytes\t path\n";
    for (size_t i = 0; i < std::min(entries.size(), size_t(10)); ++i) {
      const auto& [path, stat] = entries[i];
      ofs << stat.read_cnt << "\t" << stat.read_bytes << "\t" << stat.write_cnt
          << "\t" << stat.write_bytes << "\t" << path << "\n";
    }

    // 按 read_bytes 降序排序
    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
      return a.second.read_bytes > b.second.read_bytes;
    });
    ofs << "--- Top 10 by read bytes ---\n";
    ofs << "read_cnt\t read_bytes\t write_cnt\t write_bytes\t path\n";
    for (size_t i = 0; i < std::min(entries.size(), size_t(10)); ++i) {
      const auto& [path, stat] = entries[i];
      ofs << stat.read_cnt << "\t" << stat.read_bytes << "\t" << stat.write_cnt
          << "\t" << stat.write_bytes << "\t" << path << "\n";
    }

    // 按 write_bytes 降序排序
    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
      return a.second.write_bytes > b.second.write_bytes;
    });
    ofs << "--- Top 10 by write bytes ---\n";
    ofs << "read_cnt\t read_bytes\t write_cnt\t write_bytes\t path\n";
    for (size_t i = 0; i < std::min(entries.size(), size_t(10)); ++i) {
      const auto& [path, stat] = entries[i];
      ofs << stat.read_cnt << "\t" << stat.read_bytes << "\t" << stat.write_cnt
          << "\t" << stat.write_bytes << "\t" << path << "\n";
    }

    ofs.flush();
  }
}

void init_real_funcs() {
  real_open = (int (*)(const char*, int, ...))dlsym(RTLD_NEXT, "open");
  real_read = (ssize_t (*)(int, void*, size_t))dlsym(RTLD_NEXT, "read");
  real_write = (ssize_t (*)(int, const void*, size_t))dlsym(RTLD_NEXT, "write");
  real_close = (int (*)(int))dlsym(RTLD_NEXT, "close");
}

}  // anonymous namespace

/* ================= hook functions ================= */

extern "C" int open(const char* path, int flags, ...) {
  //   std::cout << __func__ << " called. path: " << path << "\n";

  if (!real_open) init_real_funcs();

  mode_t mode = 0;
  if (flags & O_CREAT) {
    va_list ap;
    va_start(ap, flags);
    mode = va_arg(ap, int);
    va_end(ap);
  }

  int fd = real_open(path, flags, mode);
  if (fd >= 0) {
    std::lock_guard<std::mutex> lk(g_mutex);
    g_fd_to_path[fd] = path;
  }
  return fd;
}

extern "C" ssize_t read(int fd, void* buf, size_t count) {
  //   std::cout << __func__ << " called. fd: " << fd << "\n";

  if (!real_read) init_real_funcs();
  ssize_t ret = real_read(fd, buf, count);

  if (ret > 0) {
    std::lock_guard<std::mutex> lk(g_mutex);
    auto it = g_fd_to_path.find(fd);

    std::string path;
    if (it != g_fd_to_path.end()) {
      path = it->second;
    } else {
      path = resolve_path(fd);
      g_fd_to_path[fd] = path;
    }

    auto& stat = g_stats[path];
    stat.read_cnt++;
    stat.read_bytes += ret;
  }
  return ret;
}

extern "C" ssize_t write(int fd, const void* buf, size_t count) {
  //   std::cout << __func__ << " called. fd: " << fd << "\n";

  if (!real_write) init_real_funcs();
  ssize_t ret = real_write(fd, buf, count);

  if (ret > 0) {
    std::lock_guard<std::mutex> lk(g_mutex);
    auto it = g_fd_to_path.find(fd);

    std::string path;
    if (it != g_fd_to_path.end()) {
      path = it->second;
    } else {
      path = resolve_path(fd);
      g_fd_to_path[fd] = path;
    }

    auto& stat = g_stats[path];
    stat.write_cnt++;
    stat.write_bytes += ret;
  }

  return ret;
}

extern "C" int close(int fd) {
  //   std::cout << __func__ << " called. fd: " << fd << "\n";

  if (!real_close) init_real_funcs();

  {
    std::lock_guard<std::mutex> lk(g_mutex);
    g_fd_to_path.erase(fd);
  }
  return real_close(fd);
}

/* ================= constructor / destructor ================= */

__attribute__((constructor)) static void io_monitor_init() {
  std::cout << __func__ << " called\n";
  if (std::filesystem::exists(g_log_file)) {
    std::filesystem::remove(g_log_file);
  }
  g_running.store(true);
  init_real_funcs();
  std::thread(logger_thread).detach();
}

__attribute__((destructor)) static void io_monitor_finish() {
  std::cout << __func__ << " called\n";
  g_running.store(false);
}
