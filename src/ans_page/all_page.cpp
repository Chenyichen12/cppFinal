/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 11:54:50
 */
#include "all_page.h"
#include "opeator_mat.hpp"
#include <qboxlayout.h>
#include <qlist.h>
#include <qmargins.h>
#include <qpushbutton.h>
#include <qwidget.h>

namespace Ui {
class all_page {
public:
  QPushButton *sumitBtn;
  QWidget *btnArea;
  void setupUi(QWidget *w) {
    sumitBtn = new QPushButton("提交", w);
    btnArea = new QWidget(w);
    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(btnArea);
    mainLayout->addWidget(sumitBtn);
    mainLayout->setStretch(0, 1);
    w->setLayout(mainLayout);
  }
};

}; // namespace Ui

all_page::all_page(QList<mat_ptr> showDatas, QWidget *parent)
    : QWidget(parent), ui(new Ui::all_page()) {
  this->showDatas = showDatas;
  this->ui->setupUi(this);

  auto x1Layout = new QHBoxLayout();
  auto x2Layout = new QHBoxLayout();

  for (int i = 0; i < showDatas.size(); i++) {
    auto newBtn = new all_page_mat(i, showDatas[i], ui->btnArea);
    connect(newBtn, &all_page_mat::clicked, this, &all_page::mat_has_click);
    if (i < 4) {
      x1Layout->addWidget(newBtn);
    } else {
      x2Layout->addWidget(newBtn);
    }
    matChild.push_back(newBtn);
  }

  auto ansLayout = new QVBoxLayout();
  ansLayout->setContentsMargins(QMargins(0, 0, 0, 0));
  ansLayout->addLayout(x1Layout);
  ansLayout->addLayout(x2Layout);

  ui->btnArea->setLayout(ansLayout);
}
