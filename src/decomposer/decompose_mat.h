/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-07 20:49:04
 */
#pragma once
#include <Eigen/Core>
#include <array>
#include <memory>
#include <qlist.h>
#ifdef TREE_TEST
class tree_test;
#endif
class decomposer {
#ifdef TREE_TEST
  friend tree_test;
#endif
private:
  class composer_mat;
  std::array<std::unique_ptr<composer_mat>, 7> mats;
  bool ifDecomposer;

  std::unique_ptr<Eigen::Matrix<int, 6, 6>> ans_mat;
  // 保证mat是合法的

  bool ifGo(int c, const Eigen::Matrix<int, 6, 6> &currentMat);

public:
  explicit decomposer(const Eigen::Matrix<int, 6, 6> &mat);
  void Decompose();
  bool ifDecompose() const;
  QList<Eigen::Matrix<int, 6, 6>> get_ans_mat();
};