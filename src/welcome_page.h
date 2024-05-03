/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-02 19:50:46
 */
#include <qtmetamacros.h>
#include <qwidget.h>

namespace Ui {
class welcome_page;
}
class welcome_page : public QWidget {
  Q_OBJECT
private:
  Ui::welcome_page *ui;

public:
  welcome_page(QWidget *parent = nullptr);
  ~welcome_page();

signals:
  void enter_level_mode(QString path);
  void enter_create_mode();
  void exit();
};