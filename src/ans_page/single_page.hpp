/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 18:49:18
 */
#pragma once
#include "ans_model.hpp"
#include "opeator_mat.hpp"
#include <Eigen/Core>
#include <qboxlayout.h>
#include <qlayoutitem.h>
#include <qlist.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>
#include <qstackedwidget.h>
#include <qwidget.h>
#undef uiTest
#define uiTest false
namespace Ui {
class single_page {
public:
  QStackedWidget *ansWidget;
  QWidget *toolBar;
  QPushButton *returnBtn;
  void setupUi(QWidget *w) {
    this->ansWidget = new QStackedWidget(w);

    this->toolBar = new QWidget(w);
    this->returnBtn = new QPushButton("返回", toolBar);
#if uiTest
    ansWidget->setStyleSheet("background-color: black");
#endif
    returnBtn->setMinimumWidth(60);
    auto toolBarLayout = new QVBoxLayout();
    toolBarLayout->addWidget(returnBtn);
    toolBarLayout->addSpacerItem(new QSpacerItem(10, 40, QSizePolicy::Preferred,
                                                 QSizePolicy::Expanding));
    this->toolBar->setLayout(toolBarLayout);

    auto xLayout = new QHBoxLayout();

    xLayout->addWidget(toolBar);
    xLayout->addWidget(ansWidget);
    w->setLayout(xLayout);
  }
};
}; // namespace Ui

class single_page : public QWidget {
  using mat_ptr = QSharedPointer<bool_table>;

private:
  Ui::single_page *ui;
  QList<mat_ptr> datas;

public:
  single_page(QList<mat_ptr> datas, int index = 0, QWidget *parent = nullptr)
      : QWidget(parent), ui(new Ui::single_page) {
    ui->setupUi(this);
    this->datas = datas;
    for (int i = 0; i < datas.size(); i++) {
      auto ansWidget = new opertor_mat(i, datas[i], ui->ansWidget);
      ui->ansWidget->addWidget(ansWidget);
      ui->ansWidget->setCurrentWidget(ansWidget);
    }
  }
  ~single_page() { delete ui; }
};