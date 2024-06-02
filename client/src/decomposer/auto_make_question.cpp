//
// Created by chen_yichen on 2024/5/9.
//

#include "auto_make_question.h"
#include <Eigen/Core>
#include <random>
namespace auto_make_space {
class bool_table : public Eigen::Matrix<int, 6, 6> {
public:
  int require;
  std::pair<int, int> topLeft;
  bool movenext() {
    this->setConstant(0);
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

    for (int i = 0; i < require; i++) {
      for (int j = 0; j < require; j++) {
        (*this)(i + topLeft.first, j + topLeft.second) = 1;
      }
    }

    return true;
  }
  explicit bool_table(int require) {
    this->require = require;
    this->topLeft = std::make_pair(0, 0);
    this->setConstant(0);
    for (int i = 0; i < require; i++) {
      for (int j = 0; j < require; j++) {
        (*this)(i, j) = 1;
      }
    }
  }
  void setLeftTop(std::pair<int, int> lt) {
    this->topLeft = lt;
    this->setConstant(0);
    for (int i = 0; i < require; i++) {
      for (int j = 0; j < require; j++) {
        (*this)(i + topLeft.first, j + topLeft.second) = 1;
      }
    }
  }
};
class auto_make {

private:
  std::array<bool_table, 7> t = {bool_table(4), bool_table(3), bool_table(3),
                                 bool_table(2), bool_table(2), bool_table(1),
                                 bool_table(1)};
  std::array<std::pair<int, int>, 7> t1 = {
      std::make_pair(2, 2), std::make_pair(1, 1), std::make_pair(2, 2),
      std::make_pair(0, 0), std::make_pair(2, 2), std::make_pair(3, 3),
      std::make_pair(1, 1)};
  long heCount = 0;
  void ifHeFa(const int count) {
    auto c = t[count];
    t[count] = bool_table(c.require);
    while (true) {
      auto ans = Eigen::Matrix<int, 6, 6>();
      ans.setConstant(0);
      for (int i = 0; i <= count; i++) {
        ans += t[i];
      }

      auto diFlag = true;
      int si = 0;
      for (int i = 0; i < ans.size(); i++) {
        if (ans(i) > 4) {
          diFlag = false;
          break;
        }
        if (ans(i) == 4) {
          si++;

          if (si > 1) {
            diFlag = false;
            break;
          }
        }
      }
      if (si == 0 && count == 6) {
        diFlag = false;
      }

      if (count == 6 && diFlag) {
        heCount++;
      }
      if (diFlag && count != 6)
        ifHeFa(count + 1);
      auto s = t[count].movenext();
      if (!s) {
        break;
      }
    }
  }

  bool autoGo(int count) {
    t[count].setLeftTop(t1[count]);
    while (true) {
      auto ans = Eigen::Matrix<int, 6, 6>();
      ans.setConstant(0);
      for (int i = 0; i <= count; i++) {
        ans += t[i];
      }

      auto diFlag = true;
      int si = 0;
      for (int i = 0; i < ans.size(); i++) {
        if (ans(i) > 4) {
          diFlag = false;
          break;
        }
        if (ans(i) == 4) {
          si++;

          if (si > 1) {
            diFlag = false;
            break;
          }
        }
      }
      if (si == 0 && count == 6) {
        diFlag = false;
      }

      if (count == 6 && diFlag) {
        heCount++;
        return true;
      }
      if (diFlag)
        if (autoGo(count + 1))
          return true;
      auto s = t[count].movenext();
      if (!s) {
        break;
      }
    }
    return false;
  }

public:
  auto_make() { resetRandom(); }
  bool make() { return autoGo(0); }

  void resetRandom() {
    auto random_device = std::random_device();
    auto gen = std::mt19937(random_device());
    for (int i = 0; i < 7; i++) {
      std::uniform_int_distribution<int> dis(0, 6 - t[i].require);
      t1[i].first = dis(gen);
      t1[i].second = dis(gen);
    }
  }
  Eigen::Matrix<int, 6, 6> get_question_mat() {
    auto res = Eigen::Matrix<int, 6, 6>();
    res.setConstant(0);
    for (const auto &item : this->t) {
      res += item;
    }
    return res;
  }
};
} // namespace auto_make_space

Eigen::Matrix<int, 6, 6> auto_make_question::make_one_question() {
  auto make_mat = auto_make_space::auto_make();
  while (!make_mat.make()) {
    make_mat.resetRandom();
  }
  return make_mat.get_question_mat();
}


#ifdef TREE_TEST
#include "gtest/gtest.h"
#include <iostream>
TEST(decoder,decoder1){
    for(int i = 0;i<100;i++){
        auto ans = auto_make_question::make_one_question();
        std::cout<<ans;
    }
}
#endif