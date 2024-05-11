//
// Created by chen_yichen on 2024/5/10.
//

#include "web_socket.h"
#include <QTcpSocket>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qvariant.h>
net_socket::net_socket(QObject *parent) : QObject(parent) {
  this->server = new QTcpServer(this);
  this->server->listen(QHostAddress::LocalHost, 10101);
  connect(server, SIGNAL(newConnection()), this, SLOT(on_new_connected()));
}
void net_socket::on_new_connected() {
  auto newUser = new game_users(server->nextPendingConnection(), this);
  connect(newUser, &game_users::user_init_complete, this, [=]() {
    this->clients.append(newUser);
    qDebug() << "new user" << newUser->getName();
    newUser->ackUUID();
  });
  connect(
      newUser, &game_users::user_name_change, this,
      [this](const QString &name) { qDebug() << "user name change:" << name; });
  connect(newUser, &game_users::user_score_change, this,
          [this](int score) { qDebug() << "user score change:" << score; });
}
net_socket::~net_socket() = default;
