//
// Created by chen_yichen on 2024/5/10.
//
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QTimer>
#include <QWebSocket>
#include <qapplication.h>
#include <qfile.h>
class client : public QObject {
  Q_OBJECT
  QWebSocket *socket;

public:
  client() {
    socket = new QWebSocket(QString("client"),
                            QWebSocketProtocol::VersionLatest, this);

    connect(socket, &QWebSocket::connected, this, [=]() {
      auto f = QFile(QString(json_test) + "/test.json");
      f.open(QIODeviceBase::ReadOnly);
      socket->sendTextMessage(f.readAll());
      // 定时10s后触发十
      auto i = new int(10);
      auto timer = new QTimer();
      connect(timer, &QTimer::timeout, this, [i, this, timer]() {
        if (socket->isValid()) {
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
          socket->sendTextMessage(doc.toJson());
        } else {
          timer->deleteLater();
          delete i;
        }
      });
      timer->start(100);
    });

    connect(socket, &QWebSocket::textMessageReceived, this,
            [=](const QString &data) { qDebug() << data; });

    socket->open(QUrl("ws://localhost:10101"));
  }
};
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  auto c = new client();
  return QApplication::exec();
}

#include "main.moc"