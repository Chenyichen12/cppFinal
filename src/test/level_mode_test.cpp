/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-02 10:04:43
 */
#include "../ans_page/questions/question_list.h"
#include "../ans_page/show_area/show_mat.hpp"
#include "../level_mode.h"
#include "ans_page/ans_model.hpp"
#include "gtest/gtest.h"
TEST(level_mode, testHandle1) {

  auto ans = new ans_model();
  auto questions = new question_list();
  auto q = questions->getQuestion(0);

  auto mat = new show_mat(q.mat);
  (*ans->getMat(0))(2, 3) = true;
  (*ans->getMat(1))(0, 5) = true;

  (*ans->getMat(2))(2, 2) = true;
  (*ans->getMat(2))(2, 3) = true;
  (*ans->getMat(2))(3, 2) = true;
  (*ans->getMat(2))(3, 3) = true;

  (*ans->getMat(3))(1, 0) = true;
  (*ans->getMat(3))(2, 0) = true;
  (*ans->getMat(3))(1, 1) = true;
  (*ans->getMat(3))(2, 1) = true;

  for (int i = 3; i < 6; i++) {
    for (int j = 3; j < 6; j++) {
      (*ans->getMat(4))(i, j) = true;
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 2; j < 5; j++) {
      (*ans->getMat(5))(i, j) = true;
    }
  }

  for (int i = 2; i < 6; i++) {
    for (int j = 0; j < 4; j++) {
      (*ans->getMat(6))(i, j) = true;
    }
  }

  ASSERT_TRUE(level_mode::check_correct(ans, mat));
}