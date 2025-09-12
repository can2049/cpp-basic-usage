#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

#include <iostream>
#include <string>

class ProcessLock {
 public:
  enum class LockType { READ, WRITE };

  ProcessLock(const std::string& file_path, LockType type)
      : lockfile_path_(file_path), lock_type_(type) {}

  bool acquire() {
    lock_fd_ = open(lockfile_path_.c_str(), O_CREAT | O_RDWR, 0666);
    if (lock_fd_ == -1) return false;

    int operation = (lock_type_ == LockType::READ) ? LOCK_SH : LOCK_EX;
    return flock(lock_fd_, operation) == 0;
  }

  void release() {
    if (lock_fd_ != -1) {
      flock(lock_fd_, LOCK_UN);
      close(lock_fd_);
      lock_fd_ = -1;
    }
  }

  ~ProcessLock() { release(); }

 private:
  int lock_fd_ = -1;
  std::string lockfile_path_;
  LockType lock_type_;
};

int main() {
  ProcessLock lock("/tmp/test.lock", ProcessLock::LockType::WRITE);
  bool lock_ret = lock.acquire();
  if (!lock_ret) {
    std::cerr << "Lock acquisition failed" << std::endl;
    return 1;
  }
  std::cout << "Lock acquired" << std::endl;
  lock.release();
  std::cout << "Lock released" << std::endl;
  return 0;
}
