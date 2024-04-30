/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 14:10:03
 */
#include "./game_window.hpp"
#include "qapplication.h"
#include <qobject.h>

int main(int argc, char **argv) {

#ifdef TREE_TEST

  return 0;
#else
  QApplication a(argc, argv);
  auto widget = new game_window();
  a.exec();
  return 0;
#endif
}