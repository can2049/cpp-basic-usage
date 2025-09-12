#include <fcntl.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <cstring>
#include <iostream>
#include <random>
#include <string>
#include <thread>

#include "glog/logging.h"

class FileLock {
 public:
  enum class LockType { READ, WRITE };

  FileLock(const std::string& file_path, LockType lock_type)
      : lockfile_path_(file_path), lock_type_(lock_type) {
    LOG(INFO) << __func__ << " constructor. lock type: "
              << (lock_type == LockType::READ ? "READ" : "WRITE")
              << " at: " << lockfile_path_;
  }

  FileLock(const FileLock&) = delete;
  FileLock& operator=(const FileLock&) = delete;
  FileLock(FileLock&&) = delete;
  FileLock& operator=(FileLock&&) = delete;

  ~FileLock() { release(); }

  bool acquire(bool not_block) {
    // create lock file if not exists
    lock_fd_ = open(lockfile_path_.c_str(), O_CREAT | O_RDWR, 0666);
    if (lock_fd_ == -1) {
      LOG(WARNING) << __func__ << " Failed to open lock file '"
                   << lockfile_path_ << "': " << strerror(errno);
      return false;
    }

    // set lock type
    int operation = (lock_type_ == LockType::READ) ? LOCK_SH : LOCK_EX;

    if (not_block) {
      operation |= LOCK_NB;
    }

    int lock_ret = flock(lock_fd_, operation);
    if (lock_ret == -1) {
      LOG(WARNING) << __func__ << " failed! at: " << lockfile_path_
                   << " , block? " << (!not_block)
                   << " , err: " << strerror(errno);
      return false;
    }

    // print thread id of current thread
    LOG(INFO) << __func__ << " ok! at " << lockfile_path_;
    return true;
  }

  void release() {
    if (lock_fd_ != -1) {
      int unlock_ret = flock(lock_fd_, LOCK_UN);
      if (unlock_ret == -1) {
        LOG(WARNING) << __func__ << " failed! at: " << lockfile_path_
                     << " , err: " << strerror(errno);
      } else {
        LOG(INFO) << __func__ << " ok! at: " << lockfile_path_;
      }

      close(lock_fd_);
      lock_fd_ = -1;
    }
  }

 private:
  int lock_fd_ = -1;
  const std::string lockfile_path_;
  const LockType lock_type_;
};

void test_file_lock(const std::string& lock_file_path,
                    FileLock::LockType lock_type) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis_20_40(20, 40);
  std::uniform_int_distribution<> dis_10_20(10, 20);
  FileLock lock(lock_file_path, lock_type);

  for (int i = 0; i < 10; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(dis_10_20(gen)));
    bool lock_ok = lock.acquire(true);
    if (lock_ok) {
      std::this_thread::sleep_for(std::chrono::milliseconds(dis_20_40(gen)));
      lock.release();
    }
  }
}

void test_file_lock_write_fork(FileLock::LockType lock_type1,
                               FileLock::LockType lock_type2) {
  auto pid_parent = getpid();
  const auto lock_file_path = "/tmp/test.lock." + std::to_string(pid_parent);
  LOG(INFO) << "lock_file_path: " << lock_file_path;

  pid_t pid = fork();
  if (pid == -1) {
    LOG(WARNING) << "fork failed";
    return;
  }
  if (pid == 0) {
    LOG(INFO) << "child process. " << getpid();
    test_file_lock(lock_file_path, lock_type1);
    _exit(0);
  } else {
    LOG(INFO) << "parent process. " << getpid();
    test_file_lock(lock_file_path, lock_type2);

    int status = 0;
    pid_t w = waitpid(pid, &status, 0);  // wait for that specific child
    LOG(INFO) << "waitpid: " << w << " status: " << status;
  }
}

int main() {
  LOG(INFO) << " === test file lock read / write fork "
               "==================================";
  test_file_lock_write_fork(FileLock::LockType::READ,
                            FileLock::LockType::WRITE);

  LOG(INFO) << " === test file lock write / write fork "
               "================================";
  test_file_lock_write_fork(FileLock::LockType::WRITE,
                            FileLock::LockType::WRITE);

  LOG(INFO) << " === test file lock read / read fork "
               "==================================";
  test_file_lock_write_fork(FileLock::LockType::READ, FileLock::LockType::READ);

  return 0;
}
