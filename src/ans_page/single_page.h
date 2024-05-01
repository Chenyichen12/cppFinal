/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 18:49:18
 */
#pragma once
#include "ans_model.hpp"
#include <qwidget.h>

namespace Ui {
class single_page;
}; // namespace Ui

class single_page : public QWidget {
  Q_OBJECT
  using mat_ptr = QSharedPointer<bool_table>;

private:
  Ui::single_page *ui;
  QList<mat_ptr> datas;
signals:
  void rtbClick();

public:
  single_page(QList<mat_ptr> datas, int index = 0, QWidget *parent = nullptr);

  void setAnsArea(int index);
  ~single_page();
};