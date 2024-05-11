//
// Created by chen_yichen on 2024/5/11.
//

#include "serve_model.h"
serve_model::serve_model(QObject *parent) : QObject(parent) {
  this->server = new net_socket(this);
  connect(server, SIGNAL(user_add_request(const QString &)), this,
          SLOT([this](const QString &s) {
            this->user.append(new game_users(s, this));
          }));
}
void serve_model::start_game() {
  for (auto &item : this->user) {
    item->start_game();
  }
}
const QList<game_users *> &serve_model::borrow_user() const {
  return this->user;
}
std::optional<const game_users *>
serve_model::borrow_user(const QString &id) const {
  for (const auto &item : this->user) {
    if (item->getId() == id) {
      return item;
    }
  }
  return std::nullopt;
}
void serve_model::set_user_score(game_users *u, int score) {
  u->set_score(score);
  emit user_score_change(u->getId(), score);
}
void serve_model::user_ans_three_point(const QString &id) {
  auto find = std::find_if(this->user.begin(), this->user.end(),
                           [id](auto &i) { return i->getId() == id; });
  if (find != this->user.end()) {
    this->set_user_score(*find, 3);
  }
}

void serve_model::user_ans_one_point(const QString &uuid) {
  auto find = std::find_if(this->user.begin(), this->user.end(),
                           [uuid](auto &i) { return i->getId() == uuid; });
  if (find != this->user.end()) {
    this->set_user_score(*find, 1);
  }
}
