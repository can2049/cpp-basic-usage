#include <sys/inotify.h>
#include <unistd.h>

#include <atomic>
#include <functional>
#include <string>
#include <thread>

class FileNotifier {
 public:
  using Callback = std::function<void(const std::string&)>;

  FileNotifier(const std::string& path, uint32_t mask, Callback callback);
  ~FileNotifier();

  void Start();
  void Stop();

 private:
  void MonitorLoop();

  std::string path_;

  uint32_t mask_ = 0;
  Callback callback_ = nullptr;
  int inotify_fd_ = -1;
  int watch_fd_ = -1;
  std::thread monitor_thread_;
  std::atomic<bool> running_ = false;
};
