#include <cstdio>
#include <string>

class SafeFileWriter {
 public:
  static bool write(const std::string& filePath, const std::string& content);
};
