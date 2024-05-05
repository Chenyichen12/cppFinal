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
private:
  select_page *introPage;
  QList<create_game_window *> game_widget;

public:
  create_mode(QWidget *parent = nullptr);
};
