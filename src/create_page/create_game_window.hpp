/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 14:54:17
 */
#pragma once
#include "ans_page/ans_model.hpp"
#include "create_page.hpp"
#include "show_area/show_mat.hpp"
#include "show_num_page.h"
#include <memory>
#include <qwidget.h>
#include <qwindowdefs.h>

namespace Ui {
class create_game_window {
public:
  QWidget *ansArea;
  QWidget *showArea;
  void setupUi(QWidget *w) {
    this->showArea = new QWidget(w);

    showArea->setMinimumSize(400, 400);
    auto layoutTop = new QHBoxLayout;
    layoutTop->addSpacerItem(
        new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layoutTop->addWidget(showArea);
    layoutTop->addSpacerItem(
        new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));

    ansArea = new QWidget(w);
    auto mainLayout = new QVBoxLayout();
    mainLayout->addLayout(layoutTop);
    mainLayout->addWidget(ansArea);

    w->setBaseSize(1000, 800);
    w->resize(1000, 800);
    w->setLayout(mainLayout);
  }
};
}; // namespace Ui

class create_game_window : public QWidget {
  Q_OBJECT
protected:
  Ui::create_game_window *ui;
  show_num_page *show_grid;
  create_stack *ans_area;

public:
  create_game_window(QWidget *parent = nullptr)
      : QWidget(parent), ui(new Ui::create_game_window()) {
    ui->setupUi(this);
    this->show_grid = new show_num_page(this);
    auto fix = new QVBoxLayout();
    fix->addWidget(show_grid);
    this->ui->showArea->setLayout(fix);

    this->ans_area = new create_stack(ui->ansArea);
    auto ansL = new QVBoxLayout();
    ansL->addWidget(ans_area);
    this->ui->ansArea->setLayout(ansL);

    connect(ans_area, &create_stack::submit_to_window,
            [this](ans_model *model) {
              auto mat = this->getShowMat();
              emit submit(model, mat);
            });

    connect(
        this->ans_area, &create_stack::data_change, this,
        [this](ans_model *model) { show_grid->upDateMat(model->getMat()); });
  }

  ~create_game_window() { delete ui; }

  std::shared_ptr<show_mat> getShowMat() {
    return this->show_grid->borrow_mat();
  }
signals:
  void submit(ans_model *borrowModel, std::shared_ptr<show_mat> borrowAns);
};