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
  connect(this->server, &QTcpServer::newConnection, this,
          &net_socket::on_new_connected);
  this->server->listen(QHostAddress::LocalHost, 11222);
}
void net_socket::on_new_connected() {
  auto socket = server->nextPendingConnection();
  if (socket == nullptr) {
    return;
  }
  this->clients.append(socket);

  connect(socket, SIGNAL(readyRead()), this, SLOT(on_socket_read()));
}
void net_socket::on_socket_read() {
  auto socket = qobject_cast<QTcpSocket *>(sender());
  for (int i = 0; i < this->clients.size(); i++) {
    if (this->clients[i] == socket) {
      qDebug() << "client" << i;
    }
  }
  auto error = QJsonParseError();
  QJsonDocument content = QJsonDocument::fromJson(socket->readAll(), &error);
  if (error.error != QJsonParseError::NoError) {
    qDebug() << error.errorString();
    return;
  }
  auto obj = content.object();
}
net_socket::~net_socket() = default;
