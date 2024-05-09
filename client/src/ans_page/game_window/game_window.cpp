/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 17:07:44
 */

#include "game_window.h"
#include "ans_page/ans_model.hpp"
#include "ans_page/ans_stack.hpp"
#include "ans_page/show_area/hint_dialog.h"
#include <QSpinBox>
#include <qtmetamacros.h>

class show_mat_tool : public QWidget {
  Q_OBJECT
  class dialog_with_num : public QDialog {
  public:
    QGridLayout *layout;
    QSpinBox *whichNum;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QPushButton *hint_all_btn;
    explicit dialog_with_num(QWidget *parent = nullptr) : QDialog(parent) {
      this->layout = new QGridLayout();
      this->whichNum = new QSpinBox(this);
      whichNum->setRange(1, 7);

      this->okBtn = new QPushButton("确定", this);
      this->cancelBtn = new QPushButton("取消", this);
      this->hint_all_btn = new QPushButton("提示所有", this);
      auto label = new QLabel("选择要提示的矩阵", this);
      this->layout->addWidget(label, 0, 0, 1, 3);
      this->layout->addWidget(whichNum, 1, 0, 1, 3);
      this->layout->addWidget(okBtn, 2, 0);
      this->layout->addWidget(hint_all_btn, 2, 1);
      this->layout->addWidget(cancelBtn, 2, 2);
      this->setLayout(layout);
      this->setAttribute(Qt::WA_DeleteOnClose);
    }
  };

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

    auto autoCompleteBtn = new QPushButton("自动补全", this);
    connect(autoCompleteBtn, &QPushButton::clicked, this, [this]() {
      // 一个dialog带供用户选择1-7数
      auto dialog = new dialog_with_num(this);
      connect(dialog->okBtn, &QPushButton::clicked, this, [this, dialog]() {
        auto number = dialog->whichNum->value();
        emit this->hint_mat(number - 1);
        dialog->accept();
      });
      connect(dialog->hint_all_btn, &QPushButton::clicked, this,
              [this, dialog]() {
                for (int i = 0; i < 7; i++) {
                  emit this->hint_mat(i);
                }
                dialog->accept();
              });
      connect(dialog->cancelBtn, &QPushButton::clicked, this,
              [dialog]() { dialog->reject(); });
      dialog->exec();
    });

    auto layout = new QVBoxLayout();
    layout->addWidget(hintBtn);
    layout->addWidget(autoCompleteBtn);
    this->setLayout(layout);
  }
signals:
  void hint_number(int row, int col);
  void hint_mat(int index);
};

#define uiTest false
namespace Ui {
class game_window {
public:
  QWidget *ansArea;
  QWidget *showArea;
  QLabel *whichNum;
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

    this->whichNum = new QLabel(w);
    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(whichNum);
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
  connect(show_widget_tool, SIGNAL(hint_mat(int)), this,
          SLOT(handle_hint_ans(int)));
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
void game_window::set_game_num(int num) {
  this->ui->whichNum->setText(QString("第%1关").arg(num));
}
void game_window::handle_hint_ans(int index) {
  if (this->Decomposer == nullptr) {
    this->Decomposer =
        std::make_unique<decomposer>(*this->show_grid->borrowMat());
    this->Decomposer->Decompose();
    if (!Decomposer->ifDecompose()) {
      qWarning("No answer");
      return;
    }
  }
  auto ans = Decomposer->get_ans_mat()[index];
  auto model = this->ans_area->borrowModel();
  auto mat = model->getMat(index);
  mat->setConstant(false);
  for (int i = 0; i < ans.size(); i++) {
    (*mat)(i) = ans(i) == 1;
  }
  ans_area->update();
}

#include "game_window.moc"