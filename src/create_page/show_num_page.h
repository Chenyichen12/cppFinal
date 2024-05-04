#pragma once
#include "ans_page/ans_model.hpp"
#include "show_area/show_mat.hpp"
#include <memory>
#include <qcontainerfwd.h>
#include <qlist.h>
#include <qpainter.h>
#include <qtextoption.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class show_num_page : public QWidget {
  Q_OBJECT

private:
  std::unique_ptr<show_mat> data_mat;

protected:
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

        painter.drawText(QRect(x, y, cellSize, cellSize),
                         QString("%1").arg((*this->data_mat)(row, col)));
      }
    }
  }

public:
  show_num_page(QWidget *parent = nullptr);
  show_mat *borrow_mat() { return this->data_mat.get(); }

public slots:
  void upDateMat(QList<QSharedPointer<bool_table>> lists);
};
