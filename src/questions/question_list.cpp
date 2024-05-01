/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 18:44:53
 */
#include "question_list.h"
#include <array>
#include <qfile.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlist.h>
#include <qscopedpointer.h>
#include <utility>
question_list::question_list() {
  auto path = QString(":/mat/default_mat.json");
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Cannot open file for reading");
    return;
  }

  QByteArray data = file.readAll();
  QJsonDocument doc = QJsonDocument::fromJson(data);
  auto array = doc.array();

  auto diffArray = std::array<question_struct::diffculty, 3>{
      question_struct::SIMPLE, question_struct::SIMPLE, question_struct::HARD};

  for (auto value : array) {
    auto obj = value.toObject();
    auto mat = obj.value("mat").toArray();
    auto diff = obj.value("difficulty").toInt();
    auto matArray = QList<int>();
    for (auto d : mat) {
      matArray.append(d.toInt());
    }
    this->append_question({matArray, diffArray[diff]});
  }
}

void question_list::append_question(question_struct &&data) {
  this->questions.append(std::move(data));
}

question_struct question_list::getQuestion(int index) {
  return this->questions[index];
}

int question_list::questionCount() { return this->questions.size(); }