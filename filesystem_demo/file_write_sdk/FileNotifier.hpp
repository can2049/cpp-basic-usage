#include <sys/inotify.h>
#include <unistd.h>

#include <atomic>
#include <cstring>
#include <functional>
#include <string>
#include <thread>

class FileNotifier {
 public:
  using Callback = std::function<void(const std::string&)>;

  FileNotifier(const std::string& directory, Callback callback);
  ~FileNotifier();

  void start();
  void stop();

 private:
  void monitorLoop();

  std::string directory_;
  Callback callback_ = nullptr;
  int inotifyFd_ = -1;
  int watchDescriptor_ = -1;
  std::thread monitorThread_;
  std::atomic<bool> running_ = false;
};
