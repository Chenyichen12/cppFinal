/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-07 20:49:04
 */
#pragma once
#include <Eigen/Core>
#include <qlist.h>
class composer_mat : public Eigen::Matrix<int, 6, 6> {
private:
  int require = 4;
  std::pair<int, int> topLeft;

  void initMat();

  static bool checkIfNegative(const Eigen::Matrix<int, 6, 6> &mat);

public:
  explicit composer_mat(Eigen::Matrix<int, 6, 6> mat, int require = 4);
  explicit composer_mat(int require = 4);

  void setRequire(int r);

  void resetTopLeft();
  bool checkOk(const Eigen::Matrix<int, 6, 6> &mat);
  bool moveNext();
};

#ifdef TREE_TEST
class tree_test;
#endif
class decomposer {
#ifdef TREE_TEST
  friend tree_test;
#endif
private:
  std::array<std::unique_ptr<composer_mat>, 7> mats;
  bool ifDecomposer;

  std::unique_ptr<Eigen::Matrix<int, 6, 6>> ans_mat;
  // 保证mat是合法的

  bool ifGo(int c, const Eigen::Matrix<int, 6, 6> &currentMat);

public:
  explicit decomposer(const Eigen::Matrix<int, 6, 6> &mat);
  explicit decomposer(const QList<int> &array);
  void Decompose();
  bool ifDecompose() const;
  QList<Eigen::Matrix<int, 6, 6>> get_ans_mat();
};