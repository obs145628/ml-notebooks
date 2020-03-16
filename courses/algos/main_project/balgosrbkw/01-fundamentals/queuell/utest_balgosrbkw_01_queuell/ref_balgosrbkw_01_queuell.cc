#include <iostream>
#include <vector>

void test1() {
  std::vector<int> q;
  std::cout << q.size() << std::endl;

  q.push_back(10);
  q.push_back(18);
  q.push_back(23);
  q.push_back(45);

  while (q.size()) {
    std::cout << q.front() << std::endl;
    q.erase(q.begin());
  }
}

void test2() {
  std::vector<int> q;

  q.push_back(18);
  q.push_back(25);
  q.push_back(16);
  q.push_back(56);
  std::cout << q.front() << std::endl;
  q.erase(q.begin());
  std::cout << q.front() << std::endl;
  q.erase(q.begin());

  q.push_back(12);
  std::cout << q.front() << std::endl;
  q.erase(q.begin());
  std::cout << q.front() << std::endl;
  q.erase(q.begin());

  q.push_back(24);
  q.push_back(8);
  q.push_back(-34);
  std::cout << q.front() << std::endl;
  q.erase(q.begin());
  std::cout << q.front() << std::endl;
  q.erase(q.begin());
}

void test3() {
  std::vector<int> q;
  for (int i = 0; i < 1000; ++i)
    q.push_back(3 * i * i + 2 * i - 134);

  while (q.size()) {
    std::cout << q.front() << std::endl;
    q.erase(q.begin());
  }
}

int main() {
  test1();
  test2();
  test3();
}
