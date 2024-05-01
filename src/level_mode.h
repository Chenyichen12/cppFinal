/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-01 22:31:03
 */
#include "questions/question_list.h"
#include <memory>
#include <qlabel.h>
#include <qstackedwidget.h>
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

public:
  level_mode(QString loadPath = "", QWidget *parent = nullptr);
  ~level_mode();
};
