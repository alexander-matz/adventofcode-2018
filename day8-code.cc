#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdint.h>

struct Node {
  Node() {}
  ~Node() { clear(); }
  Node(const Node&) = delete;
  void operator=(const Node&) = delete;

  void clear() {
    for (auto *child : childs) {
      delete child;
    }
    childs.clear();
    meta.clear();
  }

  int64_t sum() {
    int64_t akk = 0;
    for (int m : meta) akk += m;
    for (auto *child : childs) akk += child->sum();
    return akk;
  }

  int64_t value() {
    if (childs.size() == 0) {
      return sum();
    }
    int64_t akk = 0;
    for (int m : meta) {
      if (m <= childs.size()) {
        akk += childs[m-1]->value();
      }
    }
    return akk;
  }

  std::vector<struct Node*> childs;
  std::vector<int> meta;

  friend std::istream& operator>>(std::istream& s, Node &node) {
    int nchild;
    s >> nchild;
    int nmeta;
    s >> nmeta;
    node.clear();
    for (int i = 0; i < nchild; ++i) {
      node.childs.push_back(new Node());
      s >> *node.childs[i];
    }
    for (int i = 0; i < nmeta; ++i) {
      int tmp;
      s >> tmp;
      node.meta.push_back(tmp);
    }
    return s;
  }
};

using namespace std;

int main() {
  ifstream stream("day8-input");
  //stringstream stream("2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2");

  Node node;
  stream >> node;
  cout << "sum: " << node.sum() << "\n";
  cout << "value: " << node.value() << "\n";
}
