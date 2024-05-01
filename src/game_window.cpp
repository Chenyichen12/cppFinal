/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 17:07:44
 */

#include "game_window.h"

#define uiTest true
namespace Ui {
class game_window {
public:
  QWidget *ansArea;
  QWidget *showArea;
  void setupUi(QWidget *w) {
    this->showArea = new QWidget(w);
#if uiTest
    showArea->setObjectName("showArea");
    showArea->setStyleSheet("QWidget#showArea{background-color:white;}");
#endif
    showArea->setMinimumSize(400, 400);
    auto layoutTop = new QHBoxLayout;
    layoutTop->addSpacerItem(
        new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layoutTop->addWidget(showArea);
    layoutTop->addSpacerItem(
        new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));

    ansArea = new QWidget(w);
#if uiTest
    ansArea->setObjectName("ansArea");
    ansArea->setStyleSheet("QWidget#ansArea{background-color: white;}");
#else
    ansArea->setStyleSheet("QStackedWidget:: QFrame{border:0px}");
#endif
    auto mainLayout = new QVBoxLayout();
    mainLayout->addLayout(layoutTop);
    mainLayout->addWidget(ansArea);

    w->setBaseSize(1000, 800);
    w->resize(1000, 800);
    w->setLayout(mainLayout);
  }
};
}; // namespace Ui
game_window::game_window(QList<int> game_datas, QWidget *parent)
    : QWidget(parent), ui(new Ui::game_window) {
  ui->setupUi(this);
#ifdef uiTest
  for (auto &d : game_datas) {
    d = rand() % 5;
  }
#endif

  this->show_grid = new show_widget(game_datas, this);
  auto fix = new QVBoxLayout();
  fix->addWidget(show_grid);
  this->ui->showArea->setLayout(fix);

  this->ans_area = new ans_stack(ui->ansArea);
  auto ansL = new QVBoxLayout();
  ansL->addWidget(ans_area);
  this->ui->ansArea->setLayout(ansL);
  this->show();
}
game_window::~game_window() { delete ui; }