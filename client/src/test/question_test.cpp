/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 19:26:57
 */
#include "../ans_page/questions/question_list.h"
#include "gtest/gtest.h"

TEST(question_modelTest, test_default) {
  auto l = question_list();
  auto data = l.getQuestion(0);
  ASSERT_EQ(data.diff, question_struct::SIMPLE);
  ASSERT_EQ(data.mat[0], 0);
  ASSERT_EQ(data.mat[5], 1);
}

TEST(question_modelTest, test_default2) {
  auto l = question_list();
  auto data = l.getQuestion(1);
  ASSERT_EQ(data.diff, question_struct::SIMPLE);
  ASSERT_EQ(data.mat[0], 1);
  ASSERT_EQ(data.mat[7], 4);
}