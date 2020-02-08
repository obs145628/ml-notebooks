#include <iostream>
#include <vector>

class UnionFind {
public:
  UnionFind(int N) {
    count = N;
    id = std::vector<int>(N);
    for (int i = 0; i < N; i++)
      id[i] = i;
    sz = std::vector<int>(N);
    for (int i = 0; i < N; i++)
      sz[i] = 1;
  }

  int _count() { return count; }

  bool connected(int p, int q) { return find(p) == find(q); }

  int find(int p) {
    while (p != id[p])
      p = id[p];
    return p;
  }

  void _union(int p, int q) {
    int i = find(p);
    int j = find(q);
    if (i == j)
      return;

    if (sz[i] < sz[j]) {
      id[i] = j;
      sz[j] += sz[i];
    } else {
      id[j] = i;
      sz[i] += sz[j];
    }
    count--;
  }

private:
  std::vector<int> id;
  std::vector<int> sz;
  int count;
};

void test1() {
  UnionFind g(3);
  std::cout << g.find(0) << std::endl;
  std::cout << g.find(1) << std::endl;
  std::cout << g.find(2) << std::endl;
  std::cout << g.connected(0, 1) << std::endl;
  std::cout << g.connected(0, 2) << std::endl;
  std::cout << g.connected(1, 2) << std::endl;
  std::cout << g._count() << std::endl;
}

void test2() {
  UnionFind g(3);
  g._union(0, 1);
  std::cout << g.find(0) << std::endl;
  std::cout << g.find(1) << std::endl;
  std::cout << g.find(2) << std::endl;
  std::cout << g.connected(0, 1) << std::endl;
  std::cout << g.connected(0, 2) << std::endl;
  std::cout << g.connected(1, 2) << std::endl;
  std::cout << g._count() << std::endl;
}

void test3() {
  UnionFind g(3);
  g._union(0, 1);
  g._union(1, 2);
  std::cout << g.find(0) << std::endl;
  std::cout << g.find(1) << std::endl;
  std::cout << g.find(2) << std::endl;
  std::cout << g.connected(0, 1) << std::endl;
  std::cout << g.connected(0, 2) << std::endl;
  std::cout << g.connected(1, 2) << std::endl;
  std::cout << g._count() << std::endl;
}

void test4() {
  UnionFind g(12);
  g._union(0, 2);
  g._union(10, 5);
  g._union(4, 2);
  g._union(8, 9);
  g._union(0, 7);
  g._union(1, 7);
  g._union(3, 11);
  g._union(5, 8);
  g._union(4, 7);

  for (int i = 0; i < 12; ++i)
    std::cout << g.find(i) << std::endl;
  std::cout << g._count() << std::endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();
}
