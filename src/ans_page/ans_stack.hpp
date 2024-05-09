/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 18:37:09
 */
#pragma once
#include "./ans_model.hpp"
#include "all_page.h"
#include "single_page.h"
#include <memory>
#include <qboxlayout.h>
#include <qlist.h>
#include <qscopedpointer.h>
#include <qstackedwidget.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class ans_stack : public QStackedWidget {
  Q_OBJECT
private:
  QScopedPointer<ans_model> model;
  all_page *page1;
  single_page *page2;

public:
  explicit ans_stack(QWidget *parent = nullptr) : QStackedWidget(parent) {
    this->model.reset(new ans_model());
    this->page1 = new all_page(model->getMat(), this);

    this->page2 = new single_page(model->getMat(), 0, this);

    connect(page1, &all_page::mat_has_click, this, [this](int index) {
      this->setCurrentWidget(page2);
      page2->setAnsArea(index);
    });

    connect(page2, &single_page::rtbClick, this,
            [this]() { this->setCurrentWidget(page1); });
    this->addWidget(page1);
    this->addWidget(page2);
    this->setCurrentWidget(page1);

    // 提交按钮按下，提交模型给windows类
    connect(page1, &all_page::submit_has_click, this, [this]() {
      auto borrow = this->borrowModel();
      emit submit_to_window(borrow);
    });
  }

  ans_model *borrowModel() { return this->model.get(); }

signals:
  void submit_to_window(ans_model *model);
};