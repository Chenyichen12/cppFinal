#include "./game_window.hpp"
#include "iostream"
#include "qapplication.h"
#include <qobject.h>

int main(int argc, char **argv) {

#ifdef TREE_TEST
  std::cout << "ssss";
  return 0;
#else
  std::cout << "aaaa";
  QApplication a(argc, argv);
  auto widget = new game_window();
  a.exec();
  return 0;
#endif
}