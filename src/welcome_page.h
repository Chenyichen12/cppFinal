/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-02 19:50:46
 */
#include <qwidget.h>

namespace Ui {
class welcome_page;
}
class welcome_page : public QWidget {
private:
  Ui::welcome_page *ui;

public:
  welcome_page(QWidget *parent = nullptr);
  ~welcome_page();
};