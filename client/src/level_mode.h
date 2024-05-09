/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 22:31:03
 */
#include "./ans_page/questions/question_list.h"
#include "./ans_page/show_area/show_mat.hpp"
#include "ans_page/ans_model.hpp"
#include <memory>
#include <qlabel.h>
#include <qstackedwidget.h>
#include <qtmetamacros.h>
#include <qwidget.h>

namespace Ui {
class level_mode {
public:
  QStackedWidget *game_stack;
  QLabel *game_title;
  void setupUi(QWidget *w);
};
} // namespace Ui
class level_mode : public QWidget {
  Q_OBJECT
private:
  std::unique_ptr<question_list> questions;
  Ui::level_mode *ui;
  int current_index;

public:
  explicit level_mode(const QString &loadPath = "", QWidget *parent = nullptr);
  ~level_mode() override;
  static bool check_correct(ans_model *borrow_model, show_mat *borrow_mat);
public slots:
  void handle_submit(ans_model *borrow_model, show_mat *borrow_mat);

signals:
  void should_exit();
};
