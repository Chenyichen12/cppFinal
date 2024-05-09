/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 18:32:04
 */
#pragma once
#include <qlist.h>
struct question_struct {
  enum diffculty { SIMPLE, HARD };
  QList<int> mat;
  diffculty diff = SIMPLE;
};

class question_list {

protected:
  QList<question_struct> questions;

public:
  explicit question_list(QString dataPath = "");

  void append_question(question_struct &&data);
  void saveToDataBase(const QString &name);

  question_struct getQuestion(int index);
  int questionCount();
};