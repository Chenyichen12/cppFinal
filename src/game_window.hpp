/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 14:54:17
 */
#pragma once

#include "./show_area/show_widget.hpp"
#include "QStyle"
#include "QWidget"
#include "qstackedwidget.h"
#include <cstdlib>
#include <qboxlayout.h>
#include <qlayoutitem.h>
#include <qlist.h>
#include <qsizepolicy.h>
#include <qwidget.h>

#define uiTest true
namespace Ui {
class game_window {
public:
  QStackedWidget *ansArea;
  QWidget *showArea;
  void setupUi(QWidget *w) {
    this->showArea = new QWidget(w);
#if uiTest
    showArea->setStyleSheet("background-color:white;");
#endif
    showArea->setMinimumSize(400, 400);
    auto layoutTop = new QHBoxLayout;
    layoutTop->addSpacerItem(
        new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layoutTop->addWidget(showArea);
    layoutTop->addSpacerItem(
        new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));

    ansArea = new QStackedWidget(w);
    ansArea->setEnabled(false);
#if uiTest
    ansArea->setStyleSheet("background-color: white");
#else
    ansArea->setStyleSheet("QStackedWidget:: QFrame{border:0px}");
#endif
    auto mainLayout = new QVBoxLayout();
    mainLayout->addLayout(layoutTop);
    mainLayout->addWidget(ansArea);

    w->setBaseSize(1000, 800);
    w->resize(1000, 800);
    w->setLayout(mainLayout);
  }
};
}; // namespace Ui

class game_window : QWidget {
private:
  Ui::game_window *ui;
  show_widget *show_grid;

public:
  game_window(QList<int> game_datas = QList<int>(36, 0),
              QWidget *parent = nullptr)
      : QWidget(parent), ui(new Ui::game_window) {
    ui->setupUi(this);
#ifdef uiTest
    for (auto &d : game_datas) {
      d = rand() % 5;
    }
#endif

    this->show_grid = new show_widget(game_datas, this);
    auto fix = new QVBoxLayout();
    fix->addWidget(show_grid);
    this->ui->showArea->setLayout(fix);

    this->show();
  }
};