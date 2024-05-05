/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-05 15:49:34
 */
#include "create_mode.h"
#include "create_game_window.hpp"
#include "select_page.h"
#include <qstackedwidget.h>
#include <qwidget.h>
create_mode::create_mode(QWidget *parent) : QStackedWidget(parent) {
  this->introPage = new select_page(this);
  this->addWidget(introPage);
  connect(introPage, &select_page::add_has_click, this, [this]() {
    auto newWidget = new create_game_window(this);
    this->addWidget(newWidget);
    this->introPage->add_num_page(newWidget->getShowMat());
    this->game_widget.append(newWidget);

    connect(introPage, &select_page::widget_has_click, this, [this](int index) {
      auto ptr = game_widget[index];
      this->setCurrentWidget(ptr);
    });
    connect(newWidget, &create_game_window::submit, this,
            [this]() { this->setCurrentWidget(introPage); });
  });
}