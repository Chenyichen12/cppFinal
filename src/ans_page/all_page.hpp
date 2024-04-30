/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 17:53:44
 */
#pragma once
#include <Eigen/Core>
#include <Eigen/src/Core/Matrix.h>
#include <qlist.h>
#include <qobject.h>
#include <qwidget.h>

class all_page : public QWidget {
  using mat_ptr = QSharedPointer<Eigen::Matrix<bool, 6, 6>>;

private:
  QList<mat_ptr> showDatas;

public:
  all_page(QList<mat_ptr> showDatas, QWidget *parent = nullptr)
      : QWidget(parent) {
    this->showDatas = showDatas;
  }
};