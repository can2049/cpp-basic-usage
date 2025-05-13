#include <boost/signals2/signal.hpp>
#include <iostream>

// Slot functions
void slot1() { std::cout << "Slot 1 called!" << std::endl; }

void slot2() { std::cout << "Slot 2 called!" << std::endl; }

void slot3(int value) {
  std::cout << "Slot 3 called with value: " << value << std::endl;
}

int main() {
  // Define a signal with no arguments and no return value
  boost::signals2::signal<void()> signal_no_args;

  // Connect slot functions to the signal
  signal_no_args.connect(&slot1);
  signal_no_args.connect(&slot2);

  // Emit the signal
  std::cout << "Emitting signal_no_args:" << std::endl;
  signal_no_args();

  // Define a signal with one argument
  boost::signals2::signal<void(int)> signal_with_args;

  // Connect a slot with arguments to the signal
  signal_with_args.connect(&slot3);

  // Emit the signal with an argument
  std::cout << "Emitting signal_with_args:" << std::endl;
  signal_with_args(42);

  return 0;
}
