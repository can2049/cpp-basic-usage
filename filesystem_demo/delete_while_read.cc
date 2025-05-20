#include <chrono>
#include <filesystem>  // C++17 文件系统库
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

namespace fs = std::filesystem;

// sub thread: delete file
void deleteFile(const std::string& filename) {
  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // wait for main thread to open file
  std::cout << "[Deleter] Attempting to delete file: " << filename << "\n";
  if (fs::remove(filename)) {
    std::cout << "[Deleter] File " << filename << " deleted successfully.\n";
  } else {
    std::cerr << "[Deleter] Error deleting file!\n";
  }
}

int main() {
  const std::string filename = "/tmp/delete_while_read_test_demo.txt";

  // prepare test file (write 1MB data)
  std::ofstream outfile(filename, std::ios::binary);
  if (!outfile) {
    std::cerr << "Failed to create test file!\n";
    return 1;
  }
  std::string data(1024 * 1024, 'A');  // 1MB of 'A'
  outfile << data;
  outfile.close();
  std::cout << "Test file created with size: " << fs::file_size(filename)
            << " bytes.\n";

  // start delete thread
  std::thread deleter(deleteFile, filename);

  // main thread: read file
  std::ifstream infile(filename, std::ios::binary | std::ios::ate);
  if (!infile) {
    std::cerr << "Failed to open file for reading!\n";
    deleter.join();
    return 1;
  }

  // get file size
  size_t filesize = infile.tellg();
  infile.seekg(0);

  // read file content - read in small chunks and pause between reads
  const size_t CHUNK_SIZE = 64 * 1024;  // 64KB chunks
  std::string content;
  content.reserve(filesize);

  char buffer[CHUNK_SIZE];
  size_t totalRead = 0;

  while (infile && totalRead < filesize) {
    // read a small chunk
    size_t bytesToRead =
        std::min(CHUNK_SIZE, static_cast<size_t>(filesize) - totalRead);
    infile.read(buffer, bytesToRead);

    // check read status
    size_t bytesRead = infile.gcount();
    if (bytesRead > 0) {
      content.append(buffer, bytesRead);
      totalRead += bytesRead;
      std::cout << "[Reader] Read " << bytesRead
                << " bytes. Total: " << totalRead << "/" << filesize
                << " bytes\n";
    }

    // short pause to give delete thread more opportunity
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  infile.close();

  // check if read was complete and content is intact
  bool isComplete = (content.size() == static_cast<size_t>(filesize) &&
                     content == std::string(filesize, 'A'));
  std::cout << "File read complete. Size: " << content.size() << "/" << filesize
            << " bytes. Content is " << (isComplete ? "intact." : "corrupted!")
            << "\n";

  deleter.join();
  return 0;
}
