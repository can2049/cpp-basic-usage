#include <iostream>

#if defined CPU
const int a = 15;
#else
const int a = -15;
#endif

int main() { std::cout << a << std::endl; }
