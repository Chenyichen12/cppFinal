/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 15:58:39
 */
#pragma once
#include <Eigen/Core>
#include <qlist.h>
#include <random>
#include <stdexcept>

class show_mat : public Eigen::Matrix<int, 6, 6> {

private:
  std::unique_ptr<Eigen::Matrix<bool, 6, 6>> ifHintTable;

public:
  show_mat() { this->setZero(); }
  explicit show_mat(const QList<int> &l) {
    if (l.size() != 36) {
      throw std::invalid_argument("not 36");
    }
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 6; j++)
        (*this)(i, j) = l[i * 6 + j];
    }

    ifHintTable = std::make_unique<Eigen::Matrix<bool, 6, 6>>();
    ifHintTable->setConstant(false);
  }

  bool ifHint(int row, int col) { return (*ifHintTable)(row, col); }

  bool setHint(int row, int col) {
    if ((*ifHintTable)(row, col)) {
      return false;
    }
    (*ifHintTable)(row, col) = true;
    return true;
  }

  void setRandomHint() {
    std::random_device device;
    auto generator = std::mt19937(device());
    auto distributor = std::uniform_int_distribution<int>(0, 6);

    do {
      int row = distributor(generator);
      int col = distributor(generator);
      if (this->setHint(row, col)) {
        break;
      }
    } while (true);
  }

  void resetHint() { this->ifHintTable->setConstant(false); }
};