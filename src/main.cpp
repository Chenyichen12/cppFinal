/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 14:10:03
 */
#include "./welcome_page.h"
#include "create_page/create_mode.h"
#include "qapplication.h"
#include <qobject.h>

#ifdef TREE_TEST
#include "gtest/gtest.h"
#endif

int main(int argc, char **argv) {

#ifdef TREE_TEST

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
#else
  QApplication a(argc, argv);
  auto w = new create_mode();
  w->show();
  a.exec();
#endif
}
