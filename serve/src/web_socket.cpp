//
// Created by chen_yichen on 2024/5/10.
//

#include "web_socket.h"
#include <QJsonArray>
#include <qjsondocument.h>
#include <qvariant.h>
net_socket::net_socket(QObject *parent) : QObject(parent) {
  this->server =
      new QWebSocketServer("tree_serve", QWebSocketServer::NonSecureMode, this);

  this->server->listen(QHostAddress::LocalHost, 10101);
  connect(server, SIGNAL(newConnection()), this, SLOT(on_new_connected()));
}
void net_socket::on_new_connected() {
  auto newUser = new game_users(server->nextPendingConnection(), this);
  connect(newUser, &game_users::user_init_complete, this, [=]() {
    this->clients.append(newUser);
    newUser->ackUUID();
    auto information = QJsonDocument(this->generateUsersInformation())
                           .toJson(QJsonDocument::Compact);
    for (auto &user : this->clients) {
      //      user->send_message(QJsonDocument(information).toJson());
      user->send_message(information);
    }
  });
  connect(
      newUser, &game_users::user_name_change, this,
      [this](const QString &name) { qDebug() << "user name change:" << name; });
  connect(newUser, &game_users::user_score_change, this, [this](int score) {
    auto information = QJsonDocument(this->generateUsersInformation())
                           .toJson(QJsonDocument::Compact);
    for (auto &user : this->clients) {
      user->send_message(information);
    }
  });
}
QJsonObject net_socket::generateUsersInformation() {
  auto obj = QJsonObject();
  auto userArr = QJsonArray();
  for (auto &user : this->clients) {
    userArr.append(user->getInformation().getJSON());
  }
  obj.insert("type", user_socket_type::users_information);
  obj.insert("users", userArr);

  return obj;
}

void net_socket::show_all_users() {
  auto obj = this->generateUsersInformation();
  auto array = obj.value("users").toArray();
  for (auto &&i : array) {
    auto o = i.toObject();
    qDebug() << o;
  }
}
net_socket::~net_socket() = default;
