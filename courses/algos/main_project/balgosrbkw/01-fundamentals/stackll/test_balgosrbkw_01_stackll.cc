#include <iostream>
#include <vector>

void test1() {
  std::vector<int> s;
  std::cout << s.size() << std::endl;
  s.push_back(16);
  s.push_back(14);
  s.push_back(8);
  s.push_back(7);
  std::cout << s.size() << std::endl;

  while (!s.empty()) {
    std::cout << s.back() << std::endl;
    s.pop_back();
  }
}

void test2() {
  std::vector<int> s;
  for (int i = 0; i < 1000; ++i)
    s.push_back(2 * i * i - 12 * i + 6);
  while (!s.empty()) {
    std::cout << s.back() << std::endl;
    s.pop_back();
  }
}

int main() {
  test1();
  test2();
}
