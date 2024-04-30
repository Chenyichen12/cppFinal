/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 18:37:09
 */
#pragma once
#include "./ans_model.hpp"
#include "all_page.hpp"
#include "single_page.hpp"
#include <qboxlayout.h>
#include <qlist.h>
#include <qscopedpointer.h>
#include <qstackedwidget.h>
#include <qwidget.h>

class ans_stack : public QStackedWidget {
private:
  QScopedPointer<ans_model> model;
  all_page *page1;
  single_page *page2;

protected:
public:
  ans_stack(QWidget *parent = nullptr) : QStackedWidget(parent) {
    this->model.reset(new ans_model());
    this->page1 = new all_page(model->getMat(), this);
    QVBoxLayout *p1Layout = new QVBoxLayout();
    page1->setLayout(p1Layout);

    this->page2 = new single_page(model->getMat(), 0, this);
    this->addWidget(page2);
    this->setCurrentWidget(page2);
  }
};