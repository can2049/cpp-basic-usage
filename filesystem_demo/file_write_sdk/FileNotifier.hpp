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
  Callback callback_;
  int inotifyFd_;
  int watchDescriptor_;
  std::thread monitorThread_;
  std::atomic<bool> running_;
};
