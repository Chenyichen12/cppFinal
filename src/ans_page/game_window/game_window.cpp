/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 17:07:44
 */

#include "game_window.h"
#include "ans_page/ans_model.hpp"
#include "ans_page/ans_stack.hpp"
#include "ans_page/show_area/hint_dialog.h"
#include <qtmetamacros.h>

class show_mat_tool : public QWidget {
  Q_OBJECT
public:
  explicit show_mat_tool(QWidget *parent) : QWidget(parent) {
    auto hintBtn = new QPushButton("提示", this);
    connect(hintBtn, &QPushButton::clicked, this, [this]() {
      auto dialog = new hint_dialog(this);
      connect(dialog, &hint_dialog::ok, this, [this, dialog]() {
        auto number = dialog->get_result();
        emit hint_number(number.first, number.second);
        dialog->accept();
      });
      connect(dialog, &hint_dialog::cancel, this,
              [dialog]() { dialog->reject(); });
      dialog->exec();
    });

    auto layout = new QVBoxLayout();
    layout->addWidget(hintBtn);
    this->setLayout(layout);
  }
signals:
  void hint_number(int row, int col);
};

#define uiTest false
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
    showArea->setMinimumSize(600, 400);
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

game_window::game_window(const QList<int> &game_datas, QWidget *parent)
    : QWidget(parent), ui(new Ui::game_window) {
  ui->setupUi(this);
#if uiTest
  for (auto &d : game_datas) {
    d = rand() % 5;
  }
#endif

  this->show_grid = new show_widget(game_datas, this);
  auto show_widget_tool = new show_mat_tool(this);
  connect(show_widget_tool, SIGNAL(hint_number(int, int)), this->show_grid,
          SLOT(handleHintOneNumber(int, int)));
  show_widget_tool->setMaximumWidth(200);
  auto fix = new QHBoxLayout();
  fix->addWidget(show_grid);
  fix->addWidget(show_widget_tool);

  this->ui->showArea->setLayout(fix);

  this->ans_area = new ans_stack(ui->ansArea);
  auto ansL = new QVBoxLayout();
  ansL->addWidget(ans_area);
  this->ui->ansArea->setLayout(ansL);

  connect(ans_area, &ans_stack::submit_to_window, [this](ans_model *model) {
    auto mat = this->show_grid->borrowMat();
    emit submit(model, mat);
  });

  this->show();
}
game_window::~game_window() { delete ui; }

#include "game_window.moc"