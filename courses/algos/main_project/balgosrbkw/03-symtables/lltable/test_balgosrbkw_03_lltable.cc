#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
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

void print_arr(std::vector<int> arr) {
  std::sort(arr.begin(), arr.end());
  std::cout << '[';
  for (std::size_t i = 0; i < arr.size(); ++i) {
    std::cout << arr[i];
    if (i + 1 < arr.size())
      std::cout << ", ";
  }
  std::cout << ']' << std::endl;
}

void print_arr(std::vector<std::pair<int, int>> arr) {
  std::sort(arr.begin(), arr.end(),
            [](auto a, auto b) { return a.first < b.first; });

  std::cout << '[';
  for (std::size_t i = 0; i < arr.size(); ++i) {
    std::cout << '(' << arr[i].first << ';' << arr[i].second << ')';
    if (i + 1 < arr.size())
      std::cout << ", ";
  }
  std::cout << ']' << std::endl;
}

void print_keys(const std::map<int, int> &st) {
  std::vector<int> keys;
  for (const auto &it : st) {
    keys.push_back(it.first);
  }
  print_arr(keys);
}

void print_vals(const std::map<int, int> &st) {
  std::vector<int> vals;
  for (const auto &it : st) {
    vals.push_back(it.second);
  }
  print_arr(vals);
}

void print_table(const std::map<int, int> &st) {
  std::vector<std::pair<int, int>> vals;
  for (const auto &it : st) {
    vals.push_back(it);
  }
  print_arr(vals);
}

void printnl_int(int x) { std::cout << x << std::endl; }

int table_contains(std::map<int, int> &st, int key) {
  return st.find(key) != st.end();
}

int table_put(std::map<int, int> &st, int key, int val) {
  int res = table_contains(st, key);
  st[key] = val;
  return !res;
}

int table_delete(std::map<int, int> &st, int key) { return st.erase(key) == 1; }

void test1() {
  std::map<int, int> st;
  print_keys(st);
  print_vals(st);
  print_table(st);
}

void test2() {
  std::map<int, int> st;
  printnl_int(table_put(st, 3, 78));
  printnl_int(table_put(st, 6, 4));
  printnl_int(table_put(st, 2, 45));
  printnl_int(table_put(st, 1, 27));
  printnl_int(table_put(st, 2, 37));
  printnl_int(table_put(st, 8, 44));

  for (int i = 0; i < 10; ++i)
    printnl_int(table_contains(st, i));

  print_keys(st);
  print_vals(st);
  print_table(st);
}

void test3() {
  std::map<int, int> st;
  for (int i = 0; i < 20; ++i)
    printnl_int(table_put(st, i, i * i));

  for (int i = 0; i < 20; ++i)
    printnl_int(table_contains(st, i));
  print_table(st);

  for (int i = 0; i < 20; ++i)
    printnl_int(table_delete(st, i));
  print_table(st);
}

void test4() {
  std::map<int, int> st;

  for (int i = -40; i < 40; ++i)
    printnl_int(table_put(st, i, i * i));

  for (int i = -40; i < 40; ++i)
    printnl_int(table_contains(st, i));
  print_table(st);

  for (int i = -12; i < 4; ++i)
    printnl_int(table_delete(st, i));

  for (int i = -40; i < 40; ++i)
    printnl_int(table_contains(st, i));
  print_table(st);

  for (int i = 4; i < 28; ++i)
    printnl_int(table_put(st, i, 4 * i * i - 5));

  for (int i = -40; i < 40; ++i)
    printnl_int(table_contains(st, i));
  print_table(st);

  for (int i = -37; i < 8; ++i)
    printnl_int(table_delete(st, i));

  for (int i = -40; i < 40; ++i)
    printnl_int(table_contains(st, i));
  print_table(st);

  for (int i = 16; i < 39; ++i)
    printnl_int(table_put(st, i, -2 * i + 50));

  for (int i = -40; i < 40; ++i)
    printnl_int(table_contains(st, i));
  print_table(st);
}

int main() {
  test1();
  test2();
  test3();
  test4();
}
