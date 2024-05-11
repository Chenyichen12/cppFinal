//
// Created by chen_yichen on 2024/5/11.
//

#pragma once
#include <QJsonObject>
enum socket_event_type {
  init_user = 0,
  user_ans_one_point = 1,
  user_ans_three_point = 2,
  user_add_anno = 3
};

struct user_information {
  QString id;
  QString name;
  int score;
  virtual QJsonObject getJSON() {
    auto obj = QJsonObject();
    obj.insert("id", this->id);
    obj.insert("name", this->name);
    obj.insert("score", this->score);
  }
};