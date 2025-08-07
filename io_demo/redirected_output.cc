#include <fstream>
#include <iostream>

class Logger {
 private:
  std::ofstream logFile;
  std::streambuf* originalClogBuffer;

 public:
  Logger(const std::string& filename) {
    logFile.open(filename);
    if (!logFile) {
      std::cerr << "Failed to open log file!" << std::endl;
      return;
    }
    originalClogBuffer = std::clog.rdbuf(logFile.rdbuf());
  }

  ~Logger() {
    if (logFile.is_open()) {
      std::clog.rdbuf(originalClogBuffer);  // 恢复原始缓冲区
      logFile.close();
    }
  }
};

int main() {
  Logger logger("log.txt");  // 构造时重定向 clog
  std::clog << "This will be written to log.txt" << std::endl;
  // 析构时自动恢复
  return 0;
}
