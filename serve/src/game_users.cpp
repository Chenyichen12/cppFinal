//
// Created by chen_yichen on 2024/5/11.
//

#include "game_users.h"
#include "qtimer.h"
#include "scoket_event_type.h"
#include <QJsonDocument>
#include <QJsonObject>
game_users::game_users(QTcpSocket *socket, QObject *parent) : QObject(parent) {
  this->name = "undefined";
  this->uuid = QUuid::createUuid().toString();
  this->score = 0;
  this->tcpSocket = socket;
  tcpSocket->setParent(this);
  connect(tcpSocket, &QTcpSocket::readyRead, this, [this]() {
    auto array = this->tcpSocket->readAll();
    this->handle_socket_event(array);
  });
}

void game_users::set_name(const QString &m_name) {
  this->name = m_name;
  emit this->user_name_change(name);
}
void game_users::start_game() {
  this->start_time = QDateTime::currentDateTime();
  this->score = 0;
}
int game_users::get_score() const { return this->score; }

std::optional<qint64> game_users::during() const {
  return this->during(QDateTime::currentDateTime());
}

std::optional<qint64> game_users::during(const QDateTime &time) const {
  if (!this->start_time.has_value()) {
    return std::nullopt;
  }
  return this->start_time->msecsTo(time);
}
const QString &game_users::getId() { return this->uuid; }
void game_users::set_score(int s) {
  this->score = s;
  emit this->user_score_change(score);
}
void game_users::handle_socket_event(const QByteArray &array) {
  QJsonDocument doc = QJsonDocument::fromJson(array);
  auto obj = doc.object();
  switch (obj["type"].toInt()) {
  case socket_event_type::init_user:
    this->name = obj["name"].toString();
    emit this->user_init_complete();
    break;
  case socket_event_type::user_ans_one_point:
    this->set_score(this->score + 1);
    break;
  case socket_event_type::user_ans_three_point:
    this->set_score(this->score + 3);
    break;
  }
}
void game_users::ackUUID() {
  QJsonObject obj;
  obj.insert("type", socket_event_type::init_user);
  obj.insert("uuid", this->uuid);
  QJsonDocument doc;
  doc.setObject(obj);
  this->tcpSocket->write(doc.toJson());
}
const QString &game_users::getName() { return this->name; }
user_information game_users::getInformation() {
  auto information = user_information();
  information.name = this->name;
  information.score = this->score;
  information.id = this->uuid;
  return information;
}
