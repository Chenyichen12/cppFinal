/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 17:54:17
 */
#pragma once
#include <Eigen/Core>
#include <Eigen/src/Core/Matrix.h>
#include <array>
#include <qlist.h>
#include <qsharedpointer.h>
class bool_table : public Eigen::Matrix<bool, 6, 6> {
private:
  int require;

public:
  // TODO: 检查是否完整
  bool ifComplete() const { return false; }
  int getRequire() const { return this->require; }

  void setRequire(int require) { this->require = require; }

  bool_table(int require = 4) {
    this->setConstant(false);
    this->require = require;
  }
};
class ans_model {
  using mat_ptr = QSharedPointer<bool_table>;

private:
  QList<mat_ptr> ansList;

public:
  ans_model() {
    int area = 7;
    auto requireTable = std::array<int, 7>{1, 1, 2, 2, 3, 3, 4};
    for (int i = 0; i < area; i++) {
      auto item = new bool_table(requireTable[i]);
      item->setConstant(false);
      this->ansList.push_back(mat_ptr(item));
    }
  }

  QList<mat_ptr> getMat() const { return this->ansList; }
  mat_ptr getMat(int index) const { return this->ansList[index]; }
};