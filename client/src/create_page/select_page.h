/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-05 09:55:24
 */
#pragma once
#include "../ans_page/show_area/show_mat.hpp"
#include "show_num_page.h"
#include <memory>
#include <qevent.h>
#include <qgridlayout.h>
#include <qlist.h>
#include <qscrollarea.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class num_page_grid : public show_num_page {
  Q_OBJECT
protected:
  void mousePressEvent(QMouseEvent *event) override;

public:
  explicit num_page_grid(QWidget *parent = nullptr);
  void resetMat(std::shared_ptr<show_mat>);
signals:
  void clicked();
};
class select_page : public QScrollArea {
  Q_OBJECT
private:
  QList<num_page_grid *> show_child;
  QGridLayout *mainLayout;
  QWidget *create_grid_widget(num_page_grid *);

public:
  explicit select_page(QWidget *parent = nullptr);
  void add_num_page(std::shared_ptr<show_mat> model);
public slots:
  void handle_remake_layout(int index);
signals:
  void widget_has_click(int index);
  void add_has_click();
  void auto_add_click();
  void save_data(QList<QList<int>> &datas);

  void widget_has_delete(int index);
};