/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-07 21:24:01
 */
#include "decompose_mat.h"
#include <qdebug.h>
#include <utility>
void composer_mat::initMat() {
  for (int i = 0; i < require; i++) {
    for (int j = 0; j < require; j++) {
      (*this)(i + topLeft.first, j + topLeft.second) = 1;
    }
  }
}

bool composer_mat::checkIfNegative(const Eigen::Matrix<int, 6, 6> &mat) {
  for (int i = 0; i < mat.size(); i++) {
    if (mat(i) < 0) {
      return true;
    }
  }
  return false;
}

composer_mat::composer_mat(Eigen::Matrix<int, 6, 6> mat, int require)
    : Eigen::Matrix<int, 6, 6>(std::move(mat)) {
  this->topLeft.first = 0;
  this->topLeft.second = 0;
  this->require = require;
  initMat();
}
composer_mat::composer_mat(int require) {
  this->setConstant(0);
  this->topLeft.first = 0;
  this->topLeft.second = 0;
  this->require = require;
  initMat();
}

void composer_mat::setRequire(int r) { this->require = r; }

void composer_mat::resetTopLeft() {
  this->topLeft = std::pair<int, int>(0, 0);
  this->setConstant(0);
  for (int i = 0; i < require; i++) {
    for (int j = 0; j < require; j++) {
      (*this)(i + topLeft.first, j + topLeft.second) = 1;
    }
  }
}
bool composer_mat::checkOk(const Eigen::Matrix<int, 6, 6> &mat) {
  auto newMat = mat - (*this);
  if (checkIfNegative(newMat)) {
    return false;
  }
  return true;
}
bool composer_mat::moveNext() {
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

decomposer::decomposer(const Eigen::Matrix<int, 6, 6> &mat) {
  this->ans_mat = std::make_unique<Eigen::Matrix<int, 6, 6>>(mat);
  auto needArray = std::array<int, 7>{4, 3, 3, 2, 2, 1, 1};
  for (int i = 0; i < 7; i++) {
    mats[i] = std::make_unique<composer_mat>(needArray[i]);
  }
  this->ifDecomposer = false;
}
decomposer::decomposer(const QList<int> &array) {
  if (array.size() != 36) {
    throw std::invalid_argument("array size must be 36");
  }
  this->ans_mat =
      std::make_unique<Eigen::Matrix<int, 6, 6>>(Eigen::Matrix<int, 6, 6>());

  for (int i = 0; i < array.size(); ++i) {
    (*this->ans_mat)(i) = array[i];
  }
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
void decomposer::Decompose() { ifGo(0, *this->ans_mat); }
bool decomposer::ifDecompose() const { return this->ifDecomposer; }
QList<Eigen::Matrix<int, 6, 6>> decomposer::get_ans_mat() {
  auto res = QList<Eigen::Matrix<int, 6, 6>>();

  for (const auto &item : this->mats) {
    res.append(*item);
  }
  // 翻转这个列表
  std::reverse(res.begin(), res.end());
  return std::move(res);
}

#ifdef TREE_TEST
#include <gtest/gtest.h>
class tree_test {
public:
  static std::array<std::unique_ptr<composer_mat>, 7> getMat() {
    auto data = decomposer(Eigen::Matrix<int, 6, 6>());
    return std::move(data.mats);
  }

  static Eigen::Matrix<int, 6, 6> *get_mat_origin(decomposer *model) {
    return model->ans_mat.get();
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

TEST(decomposer, test_array_construtor) {
  auto data = QList<int>();
  for (int i = 0; i < 36; ++i) {
    data.append(i);
  }
  auto model = decomposer(data);
  auto mat = tree_test::get_mat_origin(&model);
  for (int i = 0; i < 36; i++) {
    ASSERT_EQ((*mat)(i), i);
  }

  ASSERT_EQ((*mat)(1, 1), 7);
}
#endif