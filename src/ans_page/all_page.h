/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 17:53:44
 */
#pragma once
#include "ans_model.hpp"
#include "opeator_mat.hpp"
#include <Eigen/Core>
#include <Eigen/src/Core/Matrix.h>
#include <qlist.h>
#include <qtmetamacros.h>
#include <qwidget.h>
namespace Ui {
class all_page;
} // namespace Ui

class all_page : public QWidget {
  using mat_ptr = QSharedPointer<bool_table>;
  Q_OBJECT

signals:
  void mat_has_click(int index);

private:
  QList<mat_ptr> showDatas;
  Ui::all_page *ui;
  QList<all_page_mat *> matChild;

public:
  all_page(QList<mat_ptr> showDatas, QWidget *parent = nullptr);
};