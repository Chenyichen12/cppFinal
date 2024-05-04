/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-04 19:50:08
 */
#include "show_num_page.h"
#include <qlist.h>
#include <qwidget.h>
show_num_page::show_num_page(QWidget *parent) : QWidget(parent) {
  this->data_mat.reset(new show_mat());
  this->data_mat->setConstant(0);
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