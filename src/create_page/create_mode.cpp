/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-05 15:49:34
 */
#include "create_mode.h"
#include "create_game_window.hpp"
#include "select_page.h"
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qfiledialog.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qmessagebox.h>
#include <qstackedwidget.h>
#include <qwidget.h>

create_mode::create_mode(QWidget *parent) : QStackedWidget(parent) {
  this->introPage = new select_page(this);
  this->addWidget(introPage);
  connect(introPage, &select_page::add_has_click, this, [this]() {
    auto newWidget = new create_game_window(this);
    this->addWidget(newWidget);
    this->introPage->add_num_page(newWidget->getShowMat());
    this->game_widget.append(newWidget);

    connect(introPage, &select_page::widget_has_click, this, [this](int index) {
      auto ptr = game_widget[index];
      this->setCurrentWidget(ptr);
    });
    connect(newWidget, &create_game_window::submit, this,
            [this](ans_model *model, std::shared_ptr<show_mat> mat) {
              auto res = this->checkLegial(model, mat);
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
              default:
                QMessageBox::warning(this, "错误的矩阵", "未知错误");
                return;
              }
            });
    this->setCurrentWidget(newWidget);
  });

  connect(introPage, &select_page::save_data, this, &create_mode::save_data);
}

void create_mode::save_data(QList<QList<int>> data) {
  // 获取保存位置
  auto filePath = this->request_save_path();
  if (filePath == "")
    return;
  auto mat_array = QJsonArray();
  for (int i = 0; i < data.size(); i++) {
    auto mat = QJsonArray();
    for (int j = 0; j < data[i].size(); j++) {
      mat.append(data[i][j]);
    }
    mat_array.append(mat);
  }

  QJsonDocument doc(mat_array);
  QString jsonStr(doc.toJson(QJsonDocument::Compact));

  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly)) {
    qDebug() << "Could not open file for writing";
    return;
  }

  QTextStream stream(&file);

  stream << jsonStr;
  file.close();
}

QString create_mode::request_save_path() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("保存数据"), "", tr("Json Files (*.json);;All Files (*)"));
  return std::move(fileName);
}
create_mode::check_result
create_mode::checkLegial(ans_model *model, std::shared_ptr<show_mat> mat) {
  for (auto mat : model->getMat()) {
    if (!mat->ifComplete()) {
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