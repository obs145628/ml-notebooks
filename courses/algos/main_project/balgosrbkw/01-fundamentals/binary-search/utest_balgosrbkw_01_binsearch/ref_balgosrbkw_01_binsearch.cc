#include <algorithm>
#include <iostream>
#include <vector>

int rank(const std::vector<int> &arr, int key) {
  auto it = std::find(arr.begin(), arr.end(), key);
  return it == arr.end() ? -1 : (it - arr.begin());
}

void test_empty() { std::cout << rank({}, 160) << std::endl; }

void test1() {
  std::vector<int> arr;
  for (int i = 0; i < 10; ++i)
    arr.push_back(i);

  std::cout << rank(arr, -50) << std::endl;
  for (int i = 0; i < 10; ++i)
    std::cout << rank(arr, i) << std::endl;
  std::cout << rank(arr, 63) << std::endl;
}

void test2() {
  std::vector<int> arr;
  for (int i = 0; i < 10; ++i)
    arr.push_back(i - 5);

  std::cout << rank(arr, -50) << std::endl;
  for (int i = -10; i < 10; ++i)
    std::cout << rank(arr, i) << std::endl;
  std::cout << rank(arr, 63) << std::endl;
}

void test3() {
  std::vector<int> arr;
  for (int i = 0; i < 10; ++i)
    arr.push_back(2 * i);

  std::cout << rank(arr, -50) << std::endl;
  for (int i = 0; i < 20; ++i)
    std::cout << rank(arr, i) << std::endl;
  std::cout << rank(arr, 63) << std::endl;
}

int main() {
  test_empty();
  test1();
  test2();
  test3();
}
