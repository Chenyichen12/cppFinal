//
// Created by chen_yichen on 2024/5/10.
//
#include <QTcpSocket>
#include <qapplication.h>
#include <qfile.h>

class client : public QObject {
  Q_OBJECT
  QTcpSocket *socket;

public:
  client() {
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost, 11222);
    connect(socket, &QTcpSocket::connected, this, [=]() {
      auto f = QFile(json_test);
      f.open(QIODeviceBase::ReadOnly);
      socket->write(f.readAll());
    });
  }
};
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  auto c = new client();
  return QApplication::exec();
}

#include "main.moc"