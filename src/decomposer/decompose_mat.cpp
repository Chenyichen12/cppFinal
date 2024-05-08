/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-07 21:24:01
 */
#include "decompose_mat.h"
#include <Eigen/src/Core/Matrix.h>
#include <array>
#include <memory>
#include <qdebug.h>
#include <utility>
class decomposer::composer_mat : public Eigen::Matrix<int, 6, 6> {
private:
  int require = 4;
  std::pair<int, int> topLeft;

  void initMat() {
    for (int i = 0; i < require; i++) {
      for (int j = 0; j < require; j++) {
        (*this)(i + topLeft.first, j + topLeft.second) = 1;
      }
    }
  }

  static bool checkIfNegative(const Eigen::Matrix<int, 6, 6> &mat) {
    for (int i = 0; i < mat.size(); i++) {
      if (mat(i) < 0) {
        return true;
      }
    }
    return false;
  }

public:
  explicit composer_mat(Eigen::Matrix<int, 6, 6> mat, int require = 4)
      : Eigen::Matrix<int, 6, 6>(std::move(mat)) {
    this->topLeft.first = 0;
    this->topLeft.second = 0;
    this->require = require;
    initMat();
  }
  explicit composer_mat(int require = 4) {
    this->setConstant(0);
    this->topLeft.first = 0;
    this->topLeft.second = 0;
    this->require = require;
    initMat();
  }

  void setRequire(int r) { this->require = r; }

  void resetTopLeft() {
    this->topLeft = std::pair<int, int>(0, 0);
    this->setConstant(0);
    for (int i = 0; i < require; i++) {
      for (int j = 0; j < require; j++) {
        (*this)(i + topLeft.first, j + topLeft.second) = 1;
      }
    }
  }
  bool checkOk(const Eigen::Matrix<int, 6, 6> &mat) {
    auto newMat = mat - (*this);
    if (checkIfNegative(newMat)) {
      return false;
    }
    return true;
  }
  bool moveNext() {
    if (topLeft.second == 6 - require) {
      topLeft.second = 0;
      if (topLeft.first == 6 - require) {
        return false;
      } else {
        topLeft.first++;
      }
    } else {
      topLeft.second++;
    }

    this->setConstant(0);
    for (int i = 0; i < require; i++) {
      for (int j = 0; j < require; j++) {
        (*this)(i + topLeft.first, j + topLeft.second) = 1;
      }
    }
    return true;
  }
};

decomposer::decomposer(const Eigen::Matrix<int, 6, 6> &mat) {
  this->ans_mat = std::make_unique<Eigen::Matrix<int, 6, 6>>(mat);
  auto needArray = std::array<int, 7>{4, 3, 3, 2, 2, 1, 1};
  for (int i = 0; i < 7; i++) {
    mats[i] = std::make_unique<composer_mat>(needArray[i]);
  }
  this->ifDecomposer = false;
}

bool decomposer::ifGo(int c, const Eigen::Matrix<int, 6, 6> &currentMat) {
  this->mats[c]->resetTopLeft();
  auto mat_ptr = this->mats[c].get();

  while (true) {
    if (!mat_ptr->checkOk(currentMat)) {
      auto res = mat_ptr->moveNext();
      if (!res) {
        return false;
      }
      continue;
    }
    if (c == 6) {
      return true;
    }
    if (ifGo(c + 1, currentMat - (*mat_ptr))) {
      this->ifDecomposer = true;
      return true;
    } else {
      mat_ptr->moveNext();
    }
  }
}
void decomposer::Decompose() {
  ifGo(0, *this->ans_mat);
  if (this->ifDecomposer) {
    qDebug() << "yes";
  }
}
bool decomposer::ifDecompose() const { return this->ifDecomposer; }
QList<Eigen::Matrix<int, 6, 6>> decomposer::get_ans_mat() {
  auto res = QList<Eigen::Matrix<int, 6, 6>>();

  for (const auto &item : this->mats) {
    res.append(*item);
  }
  return std::move(res);
}

#ifdef TREE_TEST
#include <gtest/gtest.h>
class tree_test {
public:
  static std::array<std::unique_ptr<decomposer::composer_mat>, 7> getMat() {
    auto data = decomposer(Eigen::Matrix<int, 6, 6>());
    return std::move(data.mats);
  }
};

TEST(decomposer, test_move) {
  auto s = tree_test::getMat();
  ASSERT_EQ((*s[0])(0, 0), 1);
  ASSERT_EQ((*s[0])(3, 3), 1);
  s[0]->moveNext();

  ASSERT_EQ((*s[0])(3, 4), 1);
  ASSERT_EQ((*s[0])(0, 0), 0);

  int i = 0;
  while (s[0]->moveNext()) {
    i++;
  }
  ASSERT_EQ(i, 7);
}

void matEqual(Eigen::Matrix<int, 6, 6> m1, Eigen::Matrix<int, 6, 6> m2) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      ASSERT_EQ(m1(i, j), m2(i, j));
    }
  }
}
TEST(decomposer, test_complete) {

  auto data = Eigen::Matrix<int, 6, 6>();
  data << 0, 1, 2, 3, 3, 1, 0, 1, 3, 4, 3, 1, 1, 2, 3, 3, 1, 0, 1, 1, 1, 1, 0,
      0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0;
  ASSERT_EQ(data(0, 0), 0);
  ASSERT_EQ(data(0, 1), 1);
  auto composer = decomposer(data);
  composer.Decompose();
  ASSERT_TRUE(composer.ifDecompose());

  auto matRes = Eigen::Matrix<int, 6, 6>();
  matRes.setConstant(0);
  for (const auto &item : composer.get_ans_mat()) {
    matRes += item;
  }
  matEqual(matRes, data);
}
#endif