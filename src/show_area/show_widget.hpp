/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 16:28:30
 */
#pragma once
#include "show_mat.hpp"
#include <memory>
#include <qcontainerfwd.h>
#include <qimage.h>

#include <qlist.h>
#include <qpainter.h>
#include <qscopedpointer.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <qwindowdefs.h>

class show_widget : public QWidget {
  Q_OBJECT
protected:
  QScopedPointer<show_mat> currentDatas;
  QImage no_tree;
  QImage two_tree;
  QImage three_tree;
  QImage four_tree;

  virtual QImage getImageFromNumber(int index) {
    switch (index) {
    case 0:
      return no_tree;
    case 1:
      return three_tree;
    case 2:
      return two_tree;
    case 3:
      return three_tree;
    case 4:
      return four_tree;
    default:
      return no_tree;
    }
  }
  void paintEvent(QPaintEvent *event) override {
    auto painter = QPainter(this);

    int gridSize = 6;
    int cellSize = 50;
    int margin = 10;
    int startX = (width() - gridSize * cellSize) / 2 - margin * 3;
    int startY = (height() - gridSize * cellSize) / 2 - margin * 3;

    for (int row = 0; row < gridSize; row++) {
      for (int col = 0; col < gridSize; col++) {
        int x = startX + col * (cellSize + margin);
        int y = startY + row * (cellSize + margin);

        auto image = this->getImageFromNumber((*this->currentDatas)(row, col));
        painter.drawImage(QRect(x, y, cellSize, cellSize), image);
      }
    }
  }

public:
  show_widget(QList<int> datas, QWidget *parent = nullptr) : QWidget(parent) {
    if (datas.length() != 36) {
      throw std::invalid_argument("Invalid data length. Expected 36.");
    }

    for (auto d : datas) {
      if (d < 0 || d > 4) {
        throw std::invalid_argument(
            "Invalid data value. Expected values between 0 and 4.");
      }
    }
    this->currentDatas.reset(new show_mat(datas));

    this->no_tree = QImage(":/icon/no_tree.png");
    this->two_tree = QImage(":/icon/two_tree.png");
    this->three_tree = QImage(":/icon/three_tree.png");
    this->four_tree = QImage(":/icon/four_tree.png");
  }

  void setDatas(QList<int> datas) {
    this->currentDatas.reset(new show_mat(datas));
    this->update();
  }

  std::shared_ptr<show_mat> getMat() const {
    return std::make_shared<show_mat>(*this->currentDatas);
  }
};
