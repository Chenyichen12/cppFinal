/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-05 10:15:03
 */
#include "select_page.h"
#include "show_area/show_mat.hpp"
#include "show_num_page.h"
#include <memory>
#include <qevent.h>
#include <qgridlayout.h>
#include <qlist.h>
#include <qmargins.h>
#include <qnamespace.h>
#include <qsize.h>
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
select_page::select_page(QWidget *parent) : QWidget(parent) {
  this->show_child = QList<num_page_grid *>();
  mainLayout = new QGridLayout();
  this->setLayout(mainLayout);
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

  int xIndex = this->show_child.size() % (row + 1);
  int yIndex = this->show_child.size() / (row + 1);

  this->mainLayout->addWidget(newWidget, yIndex, xIndex);
  this->update();
}