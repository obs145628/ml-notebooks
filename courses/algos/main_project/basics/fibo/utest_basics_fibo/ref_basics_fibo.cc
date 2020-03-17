
#include <iostream>

int fibo(int x) { return x < 2 ? x : fibo(x - 1) + fibo(x - 2); }

int main() {
  for (int i = 0; i < 10; ++i)
    std::cout << i << ": " << fibo(i) << ", " << fibo(i) << std::endl;
}
