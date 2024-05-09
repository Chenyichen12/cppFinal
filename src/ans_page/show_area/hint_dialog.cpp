//
// Created by chen_yichen on 2024/5/9.
//

#include "hint_dialog.h"
#include <QHBoxLayout>
#include <QSpinBox>
#include <qlabel.h>
#include <qpushbutton.h>
hint_dialog::hint_dialog(QWidget *parent) : QDialog(parent) {
  auto label = new QLabel("选择你想要提示的方格", this);
  // 0-6的数字选择控件

  auto row_spin = new QSpinBox(this);
  row_spin->setRange(1, 6);
  row_spin->setValue(0);
  auto col_spin = new QSpinBox(this);
  col_spin->setRange(1, 6);
  col_spin->setValue(0);

  connect(row_spin, &QSpinBox::valueChanged, this,
          [this](int value) { this->result.row = value; });
  connect(col_spin, &QSpinBox::valueChanged, this,
          [this](int value) { this->result.col = value; });

  auto submitBtn = new QPushButton("确认", this);
  auto cancelBtn = new QPushButton("取消", this);

  auto contentLayout = new QHBoxLayout();
  contentLayout->addWidget(label);
  contentLayout->addWidget(row_spin);
  contentLayout->addWidget(col_spin);

  auto btnLayout = new QHBoxLayout();
  btnLayout->addWidget(submitBtn);
  btnLayout->addWidget(cancelBtn);
  auto container = new QVBoxLayout();
  container->addLayout(contentLayout);
  container->addLayout(btnLayout);
  this->setLayout(container);

  connect(submitBtn, &QPushButton::clicked, this, [this]() { emit ok(); });

  connect(cancelBtn, &QPushButton::clicked, this, [this]() { emit cancel(); });
  setMinimumWidth(400);
  this->setAttribute(Qt::WA_DeleteOnClose);
}
hint_dialog::~hint_dialog() = default;