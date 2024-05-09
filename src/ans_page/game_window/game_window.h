/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 14:54:17
 */
#pragma once
#include "../../decomposer/decompose_mat.h"
#include "ans_page/ans_model.hpp"
#include "ans_page/ans_stack.hpp"
#include "ans_page/show_area/show_mat.hpp"
#include "ans_page/show_area/show_widget.hpp"
#include <qwidget.h>

namespace Ui {

class game_window;
}; // namespace Ui

class game_window : public QWidget {
  Q_OBJECT
private:
  std::unique_ptr<decomposer> Decomposer;

protected:
  Ui::game_window *ui;
  show_widget *show_grid;
  ans_stack *ans_area;

public:
  explicit game_window(const QList<int> &game_datas = QList<int>(36, 0),
                       QWidget *parent = nullptr);
  ~game_window() override;

  void set_game_num(int num);

public slots:
  void handle_hint_ans(int index);
signals:
  void submit(ans_model *borrowModel, show_mat *borrowAns);
};