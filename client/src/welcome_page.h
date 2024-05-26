/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-02 19:50:46
 */
#include <qstackedwidget.h>
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
  explicit welcome_page(QWidget *parent = nullptr);
  ~welcome_page() override;
signals:
  void enter_level_mode(const QString &path);
  void enter_create_mode(const std::optional<QString> &path);
  void enter_challenge_mode(const QString &);
  void exit();
};

// 用于切换页面
class main_stack : public QStackedWidget {
  Q_OBJECT
private:
  welcome_page *welcomePage;
  void return_welcome_page();

public:
  explicit main_stack(QWidget *parent = nullptr);
  ~main_stack() override;
public slots:
  void handle_enter_level_mode(const QString &path);
  void handle_enter_create_mode(const std::optional<QString> &path);
  void handle_enter_challenge_mode(const QString &);
};