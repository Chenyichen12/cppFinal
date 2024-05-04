/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-03 09:29:21
 */
#include "iostream"
#include "vector"
#include <Eigen/Core>
#include <Eigen/src/Core/Matrix.h>
#include <array>
#include <utility>
#include <vector>

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
  bool_table(int require) {
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
    if (count == 0) {
      std::cout << heCount << '\n';
    }
    if (s == false) {
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
      std::cout << ans;
      return true;
    }
    if (diFlag && count != 6)
      if (autoGo(count + 1))
        return true;
    auto s = t[count].movenext();
    if (count == 0) {
      std::cout << heCount << '\n';
    }
    if (s == false) {
      break;
    }
  }
  return false;
}
int main() {
  autoGo(0);

  //   std::cout << heCount;
}