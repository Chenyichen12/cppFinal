//
// Created by chen_yichen on 2024/5/11.
//

#include "game_users.h"
#include "qtimer.h"
game_users::game_users(QObject *parent) : QObject(parent) {
  this->name = "undefined";
  this->uuid = QUuid::createUuid().toString();
  this->score = 0;
}
game_users::game_users(const QString &name, QObject *parent) : QObject(parent) {
  this->name = name;
  this->uuid = QUuid::createUuid().toString();
  this->score = 0;
}
void game_users::set_name(const QString &m_name) { this->name = m_name; }
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
void game_users::set_score(int s) { this->score = s; }
