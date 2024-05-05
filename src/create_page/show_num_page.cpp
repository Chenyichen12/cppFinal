/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-04 19:50:08
 */
#include "show_num_page.h"
#include <qlist.h>
#include <qsize.h>
#include <qwidget.h>
show_num_page::show_num_page(QWidget *parent) : QWidget(parent) {
  this->data_mat.reset(new show_mat());
  this->data_mat->setConstant(0);
  int gridSize = 6;
  int cellSize = 50;
  int margin = 10;
  int width = gridSize * (cellSize + margin) + margin;
  this->setMinimumSize(QSize(width, width));
}

void show_num_page::upDateMat(QList<QSharedPointer<bool_table>> lists) {
  this->data_mat->setConstant(0);
  for (auto l : lists) {
    for (int i = 0; i < l->rows(); i++) {
      for (int j = 0; j < l->cols(); j++) {
        if ((*l)(i, j))
          (*this->data_mat)(i, j)++;
      }
    }
  }
  this->update();
}

void show_num_page::paintEvent(QPaintEvent *event) {
  auto painter = QPainter(this);

  int gridSize = 6;
  int cellSize = 50;
  int margin = 10;
  int startX = (width() - gridSize * cellSize) / 2 - margin;
  int startY = (height() - gridSize * cellSize) / 2 - margin;

  for (int row = 0; row < gridSize; row++) {
    for (int col = 0; col < gridSize; col++) {
      int x = startX + col * (cellSize + margin);
      int y = startY + row * (cellSize + margin);

      painter.drawText(QRect(x, y, cellSize, cellSize),
                       QString("%1").arg((*this->data_mat)(row, col)));
    }
  }
}