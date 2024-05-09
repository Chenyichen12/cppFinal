/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-05 15:49:34
 */
#include "create_mode.h"
#include "../decomposer/decompose_mat.h"
#include "create_game_window.hpp"
#include "decomposer/auto_make_question.h"
#include "select_page.h"
#include <optional>
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qfiledialog.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qmessagebox.h>
#include <qstackedwidget.h>
#include <qwidget.h>
#include <utility>
create_mode::create_mode(QWidget *parent) : QStackedWidget(parent) {
  this->introPage = new select_page(this);
  this->addWidget(introPage);
  connect(introPage, &select_page::add_has_click, this, [this]() {
    if (this->game_widget.size() >= 30) {
      QMessageBox::information(this, "不能再加力", "一个文件最多添加30个");
      return;
    }
    auto newWidget = add_game_widget();
    this->setCurrentWidget(newWidget);
  });

  connect(introPage, &select_page::save_data, this,
          &create_mode::handle_save_data);
  connect(introPage, &select_page::widget_has_delete, this, [this](int index) {
    auto widget = game_widget[index];
    game_widget.remove(index);
    this->removeWidget(widget);
    widget->deleteLater();
  });

  connect(introPage, &select_page::auto_add_click, this, [this]() {
    if (this->game_widget.size() >= 30) {
      QMessageBox::information(this, "不能再加力", "一个文件最多添加30个");
      return;
    }
    this->create_auto_make_widget();
  });
}

create_game_window *create_mode::add_game_widget() {
  auto newWidget = new create_game_window(this);
  this->addWidget(newWidget);
  this->introPage->add_num_page(newWidget->getShowMat());
  this->game_widget.append(newWidget);

  connect(introPage, &select_page::widget_has_click, this, [this](int index) {
    auto ptr = game_widget[index];
    this->setCurrentWidget(ptr);
  });
  connect(newWidget, &create_game_window::submit, this,
          [this](ans_model *model, const std::shared_ptr<show_mat> &mat) {
            auto res = create_mode::checkLegal(model, mat);
            switch (res) {
            case NO_FOUR_TREE:
              QMessageBox::warning(this, "错误的矩阵",
                                   "必须要有一个方格种植四颗树苗");

              break;
            case MORE_FOUR_TREE:
              QMessageBox::warning(this, "错误的矩阵",
                                   "多个方格种植了四颗以上的树苗");
              break;
            case MAT_NOT_COMPLETE:
              QMessageBox::warning(this, "错误的矩阵", "种植矩阵不完整");
              break;

            case COMPLETE:
              this->setCurrentWidget(introPage);
              break;
            }
          });

  return newWidget;
}

create_mode::create_mode(const QString &filePath, QWidget *parent)
    : create_mode(parent) {
  this->filePath = filePath;
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Cannot open file for reading");
    return;
  }
  QByteArray data = file.readAll();
  file.close();
  QJsonDocument doc = QJsonDocument::fromJson(data);
  auto array = doc.array();

  for (auto value : array) {
    auto matArray = QList<int>();
    auto mat = value.toArray();
    for (auto d : mat) {
      matArray.append(d.toInt());
    }
    auto de = decomposer(matArray);
    de.Decompose();
    if (de.ifDecompose()) {
      auto newWidget = add_game_widget();
      auto deModel = de.get_ans_mat();
      newWidget->setAns(deModel);
    }
  }
}
void create_mode::handle_save_data(QList<QList<int>> &data) {
  // 获取保存位置
  if (!this->filePath.has_value()) {
    filePath = this->request_save_path();
  }
  if (filePath == "") {
    filePath = std::nullopt;
    return;
  }

  auto mat_array = QJsonArray();
  for (auto &i : data) {
    auto mat = QJsonArray();
    for (int j : i) {
      mat.append(j);
    }
    mat_array.append(mat);
  }

  QJsonDocument doc(mat_array);
  QString jsonStr(doc.toJson(QJsonDocument::Compact));

  QFile file(*filePath);
  if (!file.open(QIODevice::WriteOnly)) {
    qDebug() << "Could not open file for writing";
    return;
  }

  QTextStream stream(&file);

  stream << jsonStr;
  file.close();

  // 模态提示窗口，关闭的时候发送信号
  QMessageBox::information(this, "保存成功", "保存成功");
  emit should_exit();
}

QString create_mode::request_save_path() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("保存数据"), "", tr("Json Files (*.json);;All Files (*)"));
  return std::move(fileName);
}
create_mode::check_result
create_mode::checkLegal(ans_model *model,
                        const std::shared_ptr<show_mat> &mat) {
  for (const auto &ans_mat : model->getMat()) {
    if (!ans_mat->ifComplete()) {
      return MAT_NOT_COMPLETE;
    }
  }

  int four_tree = 0;
  for (int i = 0; i < mat->rows(); i++) {
    for (int j = 0; j < mat->cols(); j++) {
      if ((*mat)(i, j) == 4) {
        four_tree++;
      }
      if ((*mat)(i, j) > 4) {
        return MORE_FOUR_TREE;
      }
    }
  }

  if (four_tree > 1) {
    return MORE_FOUR_TREE;
  }
  if (four_tree == 0) {
    return NO_FOUR_TREE;
  }

  return COMPLETE;
}
void create_mode::create_auto_make_widget() {
  auto question = auto_make_question::make_one_question();
  auto de = decomposer(question);
  de.Decompose();
  if (de.ifDecompose()) {
    auto newWidget = add_game_widget();
    auto deModel = de.get_ans_mat();
    newWidget->setAns(deModel);
  }
}
