//
// Created by chen_yichen on 2024/5/10.
//
#include "web_socket.h"
#include <qapplication.h>
#include <qwidget.h>
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  auto socket = new net_socket();
  return QApplication::exec();
}