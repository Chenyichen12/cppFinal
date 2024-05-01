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
  // 检查是否完整
  bool ifComplete() const {
    auto firstIndex = [this]() {
      for (int i = 0; i < rows(); i++) {
        for (int j = 0; j < cols(); j++) {
          if ((*this)(i, j) == true) {
            // 前表示第一个true的index位置
            return std::array<int, 2>{i, j};
          }
        }
      }
      return std::array<int, 2>{-1, -1};
    }();
    auto trueNum = [this]() {
      int count = 0;
      for (int i = 0; i < this->array().size(); i++) {
        if (this->array()(i) == true) {
          count++;
        }
      }
      return count;
    }();
    if (firstIndex[0] == -1 || trueNum != require * require) {
      return false;
    }

    for (int i = firstIndex[0]; i < require; i++) {
      for (int j = firstIndex[1]; j < require; j++) {
        if (i >= this->rows() || j >= this->cols()) {
          return false;
        }
        if (!(*this)(i, j)) {
          return false;
        }
      }
    }
    return true;
  }
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