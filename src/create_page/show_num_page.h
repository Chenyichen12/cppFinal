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
protected:
  std::shared_ptr<show_mat> data_mat;
  void paintEvent(QPaintEvent *event) override;

public:
  show_num_page(QWidget *parent = nullptr);
  std::shared_ptr<show_mat> borrow_mat() { return this->data_mat; }

public slots:
  void upDateMat(QList<QSharedPointer<bool_table>> lists);
};
