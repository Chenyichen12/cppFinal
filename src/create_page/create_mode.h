/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-05 15:34:03
 */
#pragma once
#include "create_game_window.hpp"
#include "select_page.h"
#include <qlist.h>
#include <qstackedwidget.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class create_mode : public QStackedWidget {
  Q_OBJECT
  enum check_result {
    MAT_NOT_COMPLETE,
    MORE_FOUR_TREE,
    NO_FOUR_TREE,
    COMPLETE
  };

private:
  select_page *introPage;
  QList<create_game_window *> game_widget;

public:
  create_mode(QWidget *parent = nullptr);
  check_result checkLegial(ans_model *model, std::shared_ptr<show_mat> mat);
};
