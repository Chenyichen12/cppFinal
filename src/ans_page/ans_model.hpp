/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 17:54:17
 */
#pragma once
#include <Eigen/Core>
#include <Eigen/src/Core/Matrix.h>
#include <qlist.h>
#include <qsharedpointer.h>

class ans_model {
  using mat_ptr = QSharedPointer<Eigen::Matrix<bool, 6, 6>>;

private:
  QList<mat_ptr> ansList;

public:
  ans_model() {
    int area = 7;
    for (int i = 0; i < area; i++) {
      auto item = new Eigen::Matrix<bool, 6, 6>();
      item->setConstant(false);
      this->ansList.push_back(mat_ptr(item));
    }
  }

  QList<mat_ptr> getMat() const { return this->ansList; }
  mat_ptr getMat(int index) const { return this->ansList[index]; }
};