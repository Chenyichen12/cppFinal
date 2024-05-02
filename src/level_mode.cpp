/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 22:55:15
 */
#include "level_mode.h"
#include "ans_page/ans_model.hpp"
#include "game_window/game_window.h"
#include "questions/question_list.h"
#include <qboxlayout.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qtimer.h>
#include <qwidget.h>

void Ui::level_mode::setupUi(QWidget *w) {
  this->game_stack = new QStackedWidget(w);
  this->game_title = new QLabel("闯关模式", w);
  this->game_title->setStyleSheet("color: white;");
  auto mainLayout = new QVBoxLayout();
  mainLayout->addWidget(game_title);
  mainLayout->addWidget(game_stack);
  w->setLayout(mainLayout);
}

level_mode::level_mode(QString path, QWidget *parent)
    : QWidget(parent), ui(new Ui::level_mode) {
  ui->setupUi(this);
  if (path == "") {
    this->questions.reset(new question_list());
  } else {
    this->questions.reset(new question_list(path));
  }

  for (int i = 0; i < questions->questionCount(); i++) {
    auto question = questions->getQuestion(i);
    auto game = new game_window(question.mat, this);
    ui->game_stack->addWidget(game);

    connect(game, &game_window::submit, this, &level_mode::handle_submit);
  }

  this->setObjectName("level_mode");
  this->setStyleSheet("#level_mode {background-color: white;}");
}

level_mode::~level_mode() { delete this->ui; }

void level_mode::handle_submit(ans_model *borrow_model, show_mat *borrow_mat) {
  bool ifCorrect = this->check_correct(borrow_model, borrow_mat);

  if (ifCorrect) {
    auto msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setText("作答正确");

    msgBox->addButton("下一题", QMessageBox::AcceptRole);
    auto res = msgBox->exec();
    if (res == QMessageBox::AcceptRole) {
      qDebug() << "下一题";
    }
    msgBox->exec();

  } else {
    // 作答错误
    auto msgBox =
        new QMessageBox(QMessageBox::Icon::Critical, "错误的答案", "作答错误",
                        QMessageBox::StandardButton::NoButton, this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowFlags(Qt::Window | Qt::WindowTitleHint |
                           Qt::CustomizeWindowHint);
    auto retryBtn = new QPushButton("重试(10s)", msgBox);
    retryBtn->setEnabled(false);
    msgBox->addButton(retryBtn, QMessageBox::AcceptRole);
    QTimer *timer = new QTimer(msgBox);
    int *count = new int(10);
    connect(timer, &QTimer::timeout, this, [retryBtn, count, timer]() {
      (*count)--;
      if (*count == 0) {
        retryBtn->setText(QString("重试"));
        delete count;
        timer->stop();
        retryBtn->setEnabled(true);
      } else {
        retryBtn->setText(QString("重试%1s").arg(*count));
      }
    });
    timer->start(1000);
    msgBox->exec();
  }
}
/**判断作答是否正确*/
bool level_mode::check_correct(ans_model *borrow_model, show_mat *borrow_mat) {
  auto matList = borrow_model->getMat();
  for (auto booltable : matList) {
    if (!booltable->ifComplete()) {
      return false;
    }
  }
  auto cols = matList[0]->cols();
  auto rows = matList[0]->rows();

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int count = 0;
      for (auto booltable : matList) {
        if ((*booltable)(i, j))
          count++;
      }

      if (count != (*borrow_mat)(i, j)) {
        return false;
      }
    }
  }
  return true;
}