//
// Created by chen_yichen on 2024/5/11.
//

#pragma once
#include <QJsonObject>

/*
 * {
 * type: socket_event_type
 * ......
 * }
 * */
enum socket_event_type {
  init_user = 0,
  user_ans_one_point = 1,
  user_ans_three_point = 2,
  user_add_anno = 3
};

enum user_socket_type { ack_uuid = 0, users_information = 1, start_game = 2 };

struct user_information {
  QString id;
  QString name;
  int score;
  virtual QJsonObject getJSON() {
    auto obj = QJsonObject();
    obj.insert("id", this->id);
    obj.insert("name", this->name);
    obj.insert("score", this->score);
    return obj;
  }
};