/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 15:59:01
 */
#include "../ans_page/ans_model.hpp"
#include "gtest/gtest.h"
TEST(ans_model, completeTest) {
  auto table = bool_table();
  table.setRequire(1);
  table(0, 0) = true;
  ASSERT_TRUE(table.ifComplete());

  table(0, 3) = true;
  ASSERT_FALSE(table.ifComplete());

  table = bool_table();
  table.setRequire(4);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      table(i + 1, j + 1) = true;
    }
  }

  ASSERT_TRUE(table.ifComplete());
  table(5, 5) = true;
  ASSERT_FALSE(table.ifComplete());
}