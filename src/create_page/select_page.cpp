/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-05 10:15:03
 */
#include "select_page.h"
#include "show_area/show_mat.hpp"
#include "show_num_page.h"
#include <array>
#include <memory>
#include <qboxlayout.h>
#include <qevent.h>
#include <qgridlayout.h>
#include <qlist.h>
#include <qmargins.h>
#include <qnamespace.h>
#include <qpushbutton.h>
#include <qsize.h>
#include <qsizepolicy.h>
#include <qtmetamacros.h>
#include <qwidget.h>
num_page_grid::num_page_grid(QWidget *parent) : show_num_page(parent) {}

void num_page_grid::mousePressEvent(QMouseEvent *event) {
  emit clicked();
  show_num_page::mousePressEvent(event);
}

void num_page_grid::resetMat(std::shared_ptr<show_mat> model) {
  this->data_mat = model;
}
select_page::select_page(QWidget *parent) : QScrollArea(parent) {
  this->show_child = QList<num_page_grid *>();

  mainLayout = new QGridLayout();

  auto content = new QWidget();

  auto VBoxLayout = new QVBoxLayout();
  auto add_new_btn = new QPushButton("添加", this);

  auto save_btn = new QPushButton("保存", this);
  connect(save_btn, &QPushButton::clicked, this, [this]() {
    auto datas = QList<QList<int>>();
    for (auto a : this->show_child) {
      auto mat = a->borrow_mat();
      auto list = QList<int>();
      for (int i = 0; i < mat->rows(); i++) {
        for (int j = 0; j < mat->cols(); j++) {
          list.append((*mat)(i, j));
        }
      }
      datas.append(list);
    }
    if (datas.size() == 0) {
      return;
    }
    emit this->save_data(std::move(datas));
  });
  connect(add_new_btn, &QPushButton::clicked, this,
          &select_page::add_has_click);

  VBoxLayout->addWidget(add_new_btn);
  VBoxLayout->addLayout(mainLayout);
  VBoxLayout->addWidget(save_btn);
  content->setLayout(VBoxLayout);
  this->setWidget(content);
  this->setAutoFillBackground(true);
  this->setWidgetResizable(true);
}

void select_page::add_num_page(std::shared_ptr<show_mat> model) {
  auto newWidget = new num_page_grid(this);
  newWidget->resetMat(model);
  this->show_child.append(newWidget);
  connect(newWidget, &num_page_grid::clicked, this, [=]() {
    if (newWidget == nullptr) {
      return;
    }
    for (int i = 0; i < this->show_child.size(); i++) {
      if (newWidget == this->show_child[i]) {
        emit widget_has_click(i);
      }
    }
  });

  int row = 3;

  int xIndex = (this->show_child.size() - 1) % row;
  int yIndex = (this->show_child.size() - 1) / row;

  this->mainLayout->addWidget(create_grid_widget(newWidget), yIndex, xIndex);
  this->update();
}

QWidget *select_page::create_grid_widget(num_page_grid *w) {
  auto grid_widget = new QWidget(this);
  w->setParent(grid_widget);
  auto deleteBtn = new QPushButton("删除", grid_widget);
  auto layout = new QVBoxLayout();
  layout->addWidget(w);
  layout->addWidget(deleteBtn);
  connect(deleteBtn, &QPushButton::clicked, [=]() {
    if (w == nullptr) {
      return;
    }
    for (int i = 0; i < this->show_child.size(); i++) {
      if (w == this->show_child[i]) {
        emit widget_has_delete(i);
      }
    }

    grid_widget->deleteLater();
  });

  grid_widget->setLayout(layout);
  grid_widget->setObjectName("grid");
  grid_widget->setStyleSheet("#grid{border: 1px solid black;}");
  return grid_widget;
}
