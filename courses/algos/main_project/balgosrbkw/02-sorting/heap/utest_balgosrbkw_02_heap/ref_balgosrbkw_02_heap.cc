#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

class RNG {
public:
  RNG(std::int32_t seed) : _next(seed) {}

  std::int32_t next() {
    _next = _next * 1103515245 + 12345;
    return (_next / 65536) % 32768;
  }

private:
  std::int32_t _next;
};

void print_arr(const std::vector<int> &arr) {
  std::cout << '[';
  for (std::size_t i = 0; i < arr.size(); ++i) {
    std::cout << arr[i];
    if (i + 1 < arr.size())
      std::cout << ", ";
  }
  std::cout << ']' << std::endl;
}

void test1() {
  std::vector<int> arr = {12, 8, -6, 25, 18, 12, -2};
  std::sort(arr.begin(), arr.end());
  print_arr(arr);
}

void test2() {
  std::vector<int> arr;
  for (int i = 0; i < 100; ++i)
    arr.push_back(-2 * i * i + 5 * i - 8);

  std::sort(arr.begin(), arr.end());
  print_arr(arr);
}

void test3() {
  std::vector<int> arr;
  for (int i = 0; i < 207; ++i)
    arr.push_back(1000 + 12 * i);

  std::sort(arr.begin(), arr.end());
  print_arr(arr);
}

void test4() {
  std::vector<int> arr;
  for (int i = 0; i < 178; ++i)
    arr.push_back(1000 - 12 * i);

  std::sort(arr.begin(), arr.end());
  print_arr(arr);
}

void test5() {
  std::vector<int> arr;
  RNG rng(78);
  for (int i = 0; i < 675; ++i)
    arr.push_back(rng.next());

  std::sort(arr.begin(), arr.end());
  print_arr(arr);
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
}
