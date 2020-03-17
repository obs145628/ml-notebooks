
#include <iostream>

int fact(int x) { return x < 2 ? 1 : x * fact(x - 1); }

int main() {
  for (int i = 0; i < 10; ++i)
    std::cout << i << ": " << fact(i) << ", " << fact(i) << std::endl;
}
