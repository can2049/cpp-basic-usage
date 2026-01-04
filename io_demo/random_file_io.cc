#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <random>
#include <cstdlib>
#include <cstring>

#include <filesystem>
#include <unistd.h>

namespace fs = std::filesystem;

int random_io_test() {
  // 随机数生成器
  std::random_device rd;
  std::mt19937 gen(rd());
  // 文件大小范围：1KB ~ 10MB
  std::uniform_int_distribution<size_t> size_dist(1024, 10 * 1024 * 1024);
  // 字节值范围：0 ~ 255
  std::uniform_int_distribution<unsigned char> byte_dist(0, 255);

  // 生成随机大小
  size_t file_size = size_dist(gen);
  std::cout << "Generated random file size: " << file_size << " bytes ("
            << file_size / 1024 << " KB)" << std::endl;

  // 生成临时文件路径（使用 C 风格数组，mkstemp 需要可修改缓冲区）
  char file_path[] = "/tmp/random_file_XXXXXX";
  int fd = mkstemp(file_path);
  if (fd == -1) {
    std::cerr << "Failed to create temporary file" << std::endl;
    return EXIT_FAILURE;
  }
  close(fd);  // 关闭文件描述符，我们将用 ofstream 重新打开

  std::cout << "Writing to: " << file_path << std::endl;

  // 生成随机数据并写入文件
  std::vector<unsigned char> write_data(file_size);
  for (size_t i = 0; i < file_size; ++i) {
    write_data[i] = byte_dist(gen);
  }

  {
    std::ofstream out(file_path, std::ios::binary);
    if (!out) {
      std::cerr << "Failed to open file for writing" << std::endl;
      return EXIT_FAILURE;
    }
    out.write(reinterpret_cast<const char*>(write_data.data()),
              write_data.size());
    if (!out) {
      std::cerr << "Failed to write data" << std::endl;
      return EXIT_FAILURE;
    }
    std::cout << "Write successful." << std::endl;
  }

  // 读取文件内容到内存
  std::vector<unsigned char> read_data;
  {
    std::ifstream in(file_path, std::ios::binary | std::ios::ate);
    if (!in) {
      std::cerr << "Failed to open file for reading" << std::endl;
      return EXIT_FAILURE;
    }
    std::streamsize read_size = in.tellg();
    in.seekg(0, std::ios::beg);
    read_data.resize(read_size);
    if (!in.read(reinterpret_cast<char*>(read_data.data()), read_size)) {
      std::cerr << "Failed to read data" << std::endl;
      return EXIT_FAILURE;
    }
    std::cout << "Read " << read_data.size() << " bytes." << std::endl;
  }

  // 验证数据一致性
  if (write_data.size() != read_data.size()) {
    std::cerr << "Size mismatch: write " << write_data.size() << " vs read "
              << read_data.size() << std::endl;
    return EXIT_FAILURE;
  }
  if (memcmp(write_data.data(), read_data.data(), write_data.size()) != 0) {
    std::cerr << "Data mismatch!" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Data verification passed." << std::endl;

  // 释放内存（vector 离开作用域自动释放）
  // 显式清除（可选）
  write_data.clear();
  write_data.shrink_to_fit();
  read_data.clear();
  read_data.shrink_to_fit();
  std::cout << "Memory released." << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(10));

  // 删除临时文件
  if (fs::remove(file_path)) {
    std::cout << "Temporary file removed." << std::endl;
  } else {
    std::cerr << "Failed to remove temporary file" << std::endl;
  }

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    return random_io_test();
  }

  int count = atoi(argv[1]);

  std::vector<std::thread> threads;
  for (int i = 0; i < count; ++i) {
    std::thread t(random_io_test);
    threads.push_back(std::move(t));
  }

  for (auto& t : threads) {
    t.join();
  }

  std::cout << "All tests passed." << std::endl;
  //   std::this_thread::sleep_for(std::chrono::seconds(3));
  return 0;
}
