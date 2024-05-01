/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 17:18:17
 */
#include "single_page.h"
#include "opeator_mat.hpp"
#include <qstackedwidget.h>
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

single_page::single_page(QList<mat_ptr> datas, int index, QWidget *parent)
    : QWidget(parent), ui(new Ui::single_page) {
  ui->setupUi(this);
  this->datas = datas;
  for (int i = 0; i < datas.size(); i++) {
    auto ansWidget = new touch_opertor_mat(i, datas[i], ui->ansWidget);
    ui->ansWidget->addWidget(ansWidget);
  }

  connect(this->ui->returnBtn, &QPushButton::clicked, this,
          &single_page::rtbClick);
}

void single_page::setAnsArea(int index) {
  this->ui->ansWidget->setCurrentIndex(index);
}

single_page::~single_page() { delete ui; }