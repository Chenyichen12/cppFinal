/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-04 20:14:45
 */
#pragma once
#include "../ans_page/all_page.h"
#include "../ans_page/single_page.h"
#include "ans_page/ans_model.hpp"
#include <qstackedwidget.h>
#include <qtmetamacros.h>

class create_stack : public QStackedWidget {
  Q_OBJECT
private:
  QScopedPointer<ans_model> model;
  all_page *page1;
  single_page *page2;

public:
  create_stack(QWidget *parent = nullptr) : QStackedWidget(parent) {
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

    connect(page2, &single_page::dataHasChange, this,
            [this]() { emit data_change(this->borrowModel()); });
  }

  ans_model *borrowModel() { return this->model.get(); }
signals:
  void submit_to_window(ans_model *model);
  void data_change(ans_model *model);
};