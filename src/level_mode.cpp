/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 22:55:15
 */
#include "level_mode.h"
#include "game_window/game_window.h"
#include "questions/question_list.h"
#include <qboxlayout.h>
#include <qlabel.h>
#include <qobject.h>
#include <qstackedwidget.h>
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
  }

  this->setObjectName("level_mode");
  this->setStyleSheet("#level_mode {background-color: white;}");
}

level_mode::~level_mode() { delete this->ui; }