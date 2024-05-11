//
// Created by chen_yichen on 2024/5/11.
//

#pragma once
#include "game_users.h"
#include "web_socket.h"
#include <QObject>
class serve_model : public QObject {
  Q_OBJECT
private:
  QList<game_users *> user;
  net_socket *server;
  void set_user_score(game_users *, int score);

public:
  explicit serve_model(QObject *parent = nullptr);
  const QList<game_users *> &borrow_user() const;
  std::optional<const game_users *> borrow_user(const QString &id) const;

  void start_game();

  void user_ans_one_point(const QString &uuid);
  void user_ans_three_point(const QString &uuid);
signals:
  void user_append(const QString &uuid);
  void user_remove(const QString &uuid);
  void user_score_change(const QString &uuid, int score);
};
