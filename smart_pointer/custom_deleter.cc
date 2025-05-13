#include <iostream>
#include <memory>

// 自定义资源类型
struct FileHandle {
  FILE* handle;
  FileHandle(const char* filename, const char* mode)
      : handle(fopen(filename, mode)) {
    if (!handle) throw std::runtime_error("Failed to open file");
  }
};

// 自定义删除器函数
void FileDeleter(FileHandle* fh) {
  if (fh && fh->handle) {
    fclose(fh->handle);
    std::cout << "File closed by custom deleter\n";
  }
  delete fh;
}

int main() {
  constexpr char FILE_NAME[] =
      "/tmp/test-unique-ptr-custom-deleter-file-delete-me.txt";

  // 使用函数指针作为删除器
  std::unique_ptr<FileHandle, decltype(&FileDeleter)> filePtr(
      new FileHandle(FILE_NAME, "w"), &FileDeleter);

  // 使用文件...
  fprintf(filePtr->handle, "Hello, unique_ptr with custom deleter!");

  std::cout << "File name: " << FILE_NAME << std::endl;

  // 退出作用域时自动调用FileDeleter
  return 0;
}
