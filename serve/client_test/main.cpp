//
// Created by chen_yichen on 2024/5/10.
//
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QTimer>
#include <qapplication.h>
#include <qfile.h>
class client : public QObject {
  Q_OBJECT
  QTcpSocket *socket;

public:
  client() {
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, [=]() {
      auto f = QFile(QString(json_test) + "/test.json");
      f.open(QIODeviceBase::ReadOnly);
      socket->write(f.readAll());
      // 定时10s后触发十
      auto i = new int(10);
      auto timer = new QTimer();
      connect(timer, &QTimer::timeout, this, [i, this, timer]() {
        if (socket->isOpen()) {
          *i = *i - 1;
          if (*i == 0) {
            timer->stop();
            timer->deleteLater();
            delete i;
          }
          QJsonObject obj;
          obj.insert("type", 1);
          QJsonDocument doc;
          doc.setObject(obj);
          socket->write(doc.toJson());
        } else {
          timer->deleteLater();
          delete i;
        }
      });
      timer->start(100);
    });

    connect(socket, &QTcpSocket::readyRead, this, [=]() {
      auto array = socket->readAll();
      qDebug() << array;
    });

    socket->connectToHost(QHostAddress::LocalHost, 10101);
  }
};
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  auto c = new client();
  return QApplication::exec();
}

#include "main.moc"