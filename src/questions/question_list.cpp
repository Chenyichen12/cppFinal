/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 18:44:53
 */
#include "question_list.h"
#include <qfile.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlist.h>
#include <qscopedpointer.h>
#include <utility>
question_list::question_list(QString path) {
  if (path == "") {
    path = QString(":/mat/default_mat.json");
  }
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Cannot open file for reading");
    return;
  }

  QByteArray data = file.readAll();
  file.close();
  QJsonDocument doc = QJsonDocument::fromJson(data);
  auto array = doc.array();

  for (auto value : array) {
    auto matArray = QList<int>();
    auto mat = value.toArray();
    for (auto d : mat) {
      matArray.append(d.toInt());
    }
    this->append_question({matArray, question_struct::SIMPLE});
  }
}

void question_list::append_question(question_struct &&data) {
  this->questions.append(std::move(data));
}

question_struct question_list::getQuestion(int index) {
  return this->questions[index];
}

int question_list::questionCount() { return (int)this->questions.size(); }
void question_list::saveToDataBase(const QString &name) {}
