#include <array>
#include <atomic>
#include <iostream>
#include <thread>

#define DATA_ARRAY_SIZE 10

class SideProcessor {
 public:
  SideProcessor() : data_array_{} {};

  void StartProcessing() { is_processing_ = true; };

  bool IsFinished() { return !is_processing_; };

  void Finish() { is_processing_ = false; }

  void Run(std::atomic<bool> *exit_processing) {
    while (!(*exit_processing)) {
      while (!is_processing_ && !(*exit_processing)) {
      }
      if (*exit_processing) {
        break;
      }
      ProcessData();
      Finish();
    }
  };

 private:
  std::atomic<bool> is_processing_;
  // should be something magic here.
  std::array<int, DATA_ARRAY_SIZE> data_array_;

  void ProcessData() {
    std::cout << "Side-processing data." << std::endl;
    for (unsigned int i = 0; i < 100000000; ++i) {
      for (unsigned int j = 0; j < DATA_ARRAY_SIZE; ++j) {
        data_array_[j] += static_cast<int>(is_processing_);
      }
    }
  };
};

int main() {
  std::atomic<bool> exit_processing;
  exit_processing = false;

  SideProcessor side_processor;
  std::thread thread_obj =
      std::thread(&SideProcessor::Run, &side_processor, &exit_processing);

  std::cout << "Starting side-processor." << std::endl;
  side_processor.StartProcessing();
  while (!(side_processor.IsFinished())) {
  }

  exit_processing = true;
  thread_obj.join();

  std::cout << "Done." << std::endl;

  return 0;
}
