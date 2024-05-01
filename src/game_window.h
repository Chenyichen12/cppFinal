/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 14:54:17
 */
#pragma once
#include "ans_page/ans_stack.hpp"
#include "show_area/show_widget.hpp"
#include <qwidget.h>

namespace Ui {

class game_window;
}; // namespace Ui

class game_window : QWidget {
private:
  Ui::game_window *ui;
  show_widget *show_grid;
  ans_stack *ans_area;

public:
  game_window(QList<int> game_datas = QList<int>(36, 0),
              QWidget *parent = nullptr);

  ~game_window();
};