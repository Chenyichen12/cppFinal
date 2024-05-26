//
// Created by chen_yichen on 2024/5/9.
//
#include "iostream"
#include "memory"
class un {
public:
  un(const un &un1) { this->ptr = std::make_unique<int>(*un1.ptr); }
  std::unique_ptr<int> ptr;
  un() { this->ptr = std::make_unique<int>(10); }
};

int main() {
  const un &test = un();
  std::cout << *test.ptr;
  const un &t2 = test;
  *t2.ptr = 20;
}
class s {
  int gg = 1;
  int &sss() { return gg; }
};

class in {
  int &shui;
  in(int &data) : shui(data) {}
  void mo(int &ano) { this->shui = ano; }
};