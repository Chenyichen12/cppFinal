/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 15:58:39
 */
#pragma once
#include <Eigen/Core>
#include <qlist.h>
#include <stdexcept>

class show_mat : public Eigen::Matrix<int, 6, 6> {
public:
  show_mat() { this->setZero(); }
  show_mat(QList<int> l) {
    if (l.size() != 36) {
      throw std::invalid_argument("not 36");
    }
    for (int i = 0; i < 36; i++) {
      (*this)(i) = l[i];
    }
  }
};