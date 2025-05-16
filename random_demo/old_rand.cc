#include <cstdlib>
#include <ctime>
#include <iostream>

class RandomLost {
 public:
  RandomLost(int seed, float lost_ratio)
      : seed_(seed), lost_ratio_(lost_ratio) {
    std::srand(seed_);
  }

  bool should_lost() {
    constexpr int max_value = 10000;
    int random_value = std::rand() % max_value;
    int max_bound = max_value * lost_ratio_;
    return max_bound < random_value;
  }

 private:
  int seed_ = 999;
  float lost_ratio_ = 0.01;
};

void test_random_lost() {
  RandomLost random_lost(9999, 0.5);
  for (int i = 0; i < 10; ++i) {
    std::cout << "Lost: " << random_lost.should_lost() << std::endl;
  }
}

int roll() {
  std::srand(999);  // use current time as seed for random generator
  int random_value = std::rand();
  std::cout << "Random value on [0, " << RAND_MAX << "]: " << random_value
            << "\n"
               "Roll 6-sided dice 20 times: ";
  for (int n = 0; n != 20; ++n) {
    int x = 7;
    while (x > 6)
      x = 1 + std::rand() /
                  ((RAND_MAX + 1u) / 6);  // Note: 1 + rand() % 6 is biased
    std::cout << x << ' ';
  }
  std::cout << '\n';
  return 0;
}

int main() { test_random_lost(); }
