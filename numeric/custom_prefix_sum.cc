

#include <functional>  // For custom binary operations
#include <iostream>
#include <numeric>  // For std::inclusive_scan
#include <vector>

// Define the Data class
class Data {
 public:
  int dis() const { return dis_; }

  void set_dis(int dis) { dis_ = dis; }

 private:
  int dis_;
};

int main() {
  // Create some Data objects
  std::vector<Data> data = {Data(), Data(), Data(), Data(), Data()};

  // Set values for dis_ for each Data object
  data[0].set_dis(1);
  data[1].set_dis(2);
  data[2].set_dis(3);
  data[3].set_dis(4);
  data[4].set_dis(5);

  std::cout << " original input: ";
  for (const auto& val : data) {
    std::cout << val.dis() << " ";
  }
  std::cout << std::endl;

  // Vector to store the result of the inclusive scan
  std::vector<Data> result(data.size());

  // Perform the inclusive scan using the dis() function to accumulate the dis_
  // values
  std::inclusive_scan(data.begin(), data.end(), result.begin(),
                      [](const Data& a, const Data& b) -> Data {
                        // Accumulate the dis_ values
                        Data new_data;
                        new_data.set_dis(a.dis() + b.dis());
                        return new_data;
                      });

  // Output the result
  std::cout << "Inclusive scan result (prefix sum of dis_): ";
  for (const auto& val : result) {
    std::cout << val.dis() << " ";
  }
  std::cout << std::endl;

  return 0;
}
