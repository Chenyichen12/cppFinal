/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-05 15:34:03
 */
#pragma once
#include "create_game_window.hpp"
#include "select_page.h"
#include <optional>
#include <qlist.h>
#include <qstackedwidget.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class create_mode : public QStackedWidget {
  Q_OBJECT
  enum check_result {
    MAT_NOT_COMPLETE,
    MORE_FOUR_TREE,
    NO_FOUR_TREE,
    COMPLETE
  };

private:
  select_page *introPage;
  QList<create_game_window *> game_widget;
  QString request_save_path();
  std::optional<QString> filePath;

  create_game_window *add_game_widget();

  void handle_save_data(QList<QList<int>> &data);

  void create_auto_make_widget();

public:
  explicit create_mode(QWidget *parent = nullptr);
  explicit create_mode(const QString &file_path, QWidget *parent = nullptr);
  static check_result checkLegal(ans_model *model,
                                 const std::shared_ptr<show_mat> &mat);
signals:
  void should_exit();
};
