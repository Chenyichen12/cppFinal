/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-02 19:55:06
 */
#include "welcome_page.h"
#include <qboxlayout.h>
#include <qfont.h>
#include <qlabel.h>
#include <qlayoutitem.h>
#include <qmargins.h>
#include <qpushbutton.h>
#include <qsize.h>
#include <qsizepolicy.h>
#include <qwidget.h>

namespace Ui {
class welcome_page {
public:
  QLabel *label;
  QPushButton *level_mode_btn;
  QPushButton *create_mode_btn;
  QPushButton *exit_btn;

  void setupUi(QWidget *w) {
    auto titleWidget = new QWidget(w);
    label = new QLabel("植此青绿", titleWidget);
    auto font = QFont();
    font.setPointSize(30);
    label->setFont(font);
    auto labelLayout = new QHBoxLayout();
    labelLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                               QSizePolicy::Preferred));

    labelLayout->addWidget(label);
    labelLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                               QSizePolicy::Preferred));

    titleWidget->setLayout(labelLayout);
    auto fontSize = QFont();
    fontSize.setPointSize(20);
    level_mode_btn = new QPushButton("闯关模式", w);
    level_mode_btn->setMinimumSize(QSize(300, 50));
    level_mode_btn->setFont(fontSize);

    auto levelLayout = new QHBoxLayout();
    levelLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                               QSizePolicy::Preferred));
    levelLayout->addWidget(level_mode_btn);
    levelLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                               QSizePolicy::Preferred));

    create_mode_btn = new QPushButton("创建模式", w);
    create_mode_btn->setMinimumSize(QSize(300, 50));
    create_mode_btn->setFont(fontSize);
    auto createLayout = new QHBoxLayout();
    createLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                                QSizePolicy::Preferred));
    createLayout->addWidget(create_mode_btn);
    createLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                                QSizePolicy::Preferred));

    exit_btn = new QPushButton("退出", w);
    exit_btn->setMinimumSize(QSize(300, 50));
    exit_btn->setFont(fontSize);
    auto exitLayout = new QHBoxLayout();
    exitLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                              QSizePolicy::Preferred));
    exitLayout->addWidget(exit_btn);
    exitLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                              QSizePolicy::Preferred));

    auto container_layout = new QVBoxLayout();
    container_layout->setContentsMargins(QMargins(20, 20, 20, 20));
    container_layout->setSpacing(20);

    container_layout->addLayout(levelLayout);
    container_layout->addLayout(createLayout);
    container_layout->addLayout(exitLayout);

    auto widgetContainer = new QWidget(w);
    widgetContainer->setLayout(container_layout);
    auto main_layout = new QVBoxLayout();
    main_layout->addWidget(titleWidget);
    main_layout->addWidget(widgetContainer);

    w->setLayout(main_layout);
  }
};
} // namespace Ui

welcome_page::welcome_page(QWidget *parent)
    : QWidget(parent), ui(new Ui::welcome_page()) {
  ui->setupUi(this);
}

welcome_page::~welcome_page() { delete this->ui; }