//
// Created by chen_yichen on 2024/5/10.
//
#include "serve_model.h"
#include "web_socket.h"
#include <qapplication.h>
#include <qwidget.h>
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  auto model = new serve_model();
  return QApplication::exec();
}