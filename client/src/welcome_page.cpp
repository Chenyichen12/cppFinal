/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-05-02 19:55:06
 */
#include "welcome_page.h"
#include "create_page/create_mode.h"
#include "level_mode.h"
#include "web_challenge/challenge_mode.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <qapplication.h>
#include <qboxlayout.h>
#include <qfont.h>
#include <qlabel.h>
#include <qlayoutitem.h>
#include <qmargins.h>
#include <qpushbutton.h>
#include <qsize.h>
#include <qsizepolicy.h>
#include <qwidget.h>
namespace Ui {
class welcome_page {
public:
  QLabel *label;
  QPushButton *level_mode_btn;
  QPushButton *create_mode_btn;
  QPushButton *exit_btn;
  QPushButton *challenge_btn;

  void setupUi(QWidget *w) {
    auto titleWidget = new QWidget(w);
    label = new QLabel("植此青绿", titleWidget);
    auto font = QFont();
    font.setPointSize(30);
    label->setFont(font);
    auto labelLayout = new QHBoxLayout();
    labelLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                               QSizePolicy::Preferred));

    labelLayout->addWidget(label);
    labelLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                               QSizePolicy::Preferred));

    titleWidget->setLayout(labelLayout);
    auto fontSize = QFont();
    fontSize.setPointSize(20);
    level_mode_btn = new QPushButton("闯关模式", w);
    level_mode_btn->setMinimumSize(QSize(300, 50));
    level_mode_btn->setFont(fontSize);

    auto levelLayout = new QHBoxLayout();
    levelLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                               QSizePolicy::Preferred));
    levelLayout->addWidget(level_mode_btn);
    levelLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                               QSizePolicy::Preferred));

    create_mode_btn = new QPushButton("创建模式", w);
    create_mode_btn->setMinimumSize(QSize(300, 50));
    create_mode_btn->setFont(fontSize);
    auto createLayout = new QHBoxLayout();
    createLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                                QSizePolicy::Preferred));
    createLayout->addWidget(create_mode_btn);
    createLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                                QSizePolicy::Preferred));

    exit_btn = new QPushButton("退出", w);
    exit_btn->setMinimumSize(QSize(300, 50));
    exit_btn->setFont(fontSize);
    auto exitLayout = new QHBoxLayout();
    exitLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                              QSizePolicy::Preferred));
    exitLayout->addWidget(exit_btn);
    exitLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding,
                                              QSizePolicy::Preferred));

    challenge_btn = new QPushButton("挑战模式", w);
    challenge_btn->setMinimumSize(QSize(300, 50));
    challenge_btn->setFont(fontSize);
    auto challengeLayout = new QHBoxLayout();
    challengeLayout->addSpacerItem(new QSpacerItem(
        20, 10, QSizePolicy::Expanding, QSizePolicy::Preferred));
    challengeLayout->addWidget(challenge_btn);
    challengeLayout->addSpacerItem(new QSpacerItem(
        20, 10, QSizePolicy::Expanding, QSizePolicy::Preferred));

    auto container_layout = new QVBoxLayout();
    container_layout->setContentsMargins(QMargins(20, 20, 20, 20));
    container_layout->setSpacing(20);

    container_layout->addLayout(levelLayout);
    container_layout->addLayout(createLayout);

    container_layout->addLayout(challengeLayout);
    container_layout->addLayout(exitLayout);
    auto widgetContainer = new QWidget(w);
    widgetContainer->setLayout(container_layout);
    auto main_layout = new QVBoxLayout();
    main_layout->addWidget(titleWidget);
    main_layout->addWidget(widgetContainer);

    w->setLayout(main_layout);
  }
};
} // namespace Ui
class challenge_dialog : public QDialog {
  Q_OBJECT
public:
  QString name;
  challenge_dialog() {
    auto textEdit = new QTextEdit(this);
    auto *label = new QLabel("请输入你的名字", this);
    auto layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(textEdit);
    auto okBtn = new QPushButton("确定", this);
    auto cancelBtn = new QPushButton("取消", this);
    auto x = new QHBoxLayout();
    x->addWidget(okBtn);
    x->addWidget(cancelBtn);
    layout->addLayout(x);
    this->setLayout(layout);

    connect(textEdit, &QTextEdit::textChanged,
            [this, textEdit]() { this->name = textEdit->toPlainText(); });
    connect(okBtn, &QPushButton::clicked, this, &challenge_dialog::ok);
    connect(cancelBtn, &QPushButton::clicked, this, &challenge_dialog::cancle);
    this->setAttribute(Qt::WA_DeleteOnClose);
  }
signals:
  void ok();
  void cancle();
};

welcome_page::welcome_page(QWidget *parent)
    : QWidget(parent), ui(new Ui::welcome_page()) {
  ui->setupUi(this);
  connect(ui->level_mode_btn, &QPushButton::clicked, [this]() {
    auto path =
        QFileDialog::getOpenFileName(this, "打开文件", "", "json文件(*.json)");
    emit enter_level_mode(path);
  });
  connect(ui->create_mode_btn, &QPushButton::clicked, this, [this]() {
    // 显示从文件导入还是新建对话框
    auto d = new QMessageBox(this);
    d->setWindowTitle("是否从文件导入");
    // 设置文本
    d->setText("是否导入题目");
    d->addButton("打开题目json文件", QMessageBox::AcceptRole);
    d->addButton("新建题目文件", QMessageBox::RejectRole);
    // 自动析构
    d->setAttribute(Qt::WA_DeleteOnClose);
    auto role = d->exec();
    if (role == QMessageBox::AcceptRole) {
      // 打开文件
      auto path = QFileDialog::getOpenFileName(this, "打开文件", "",
                                               "json文件(*.json)");
      if (path == "") {
        emit enter_create_mode(std::nullopt);
      } else {
        emit enter_create_mode(path);
      }
    } else {
      emit enter_create_mode(std::nullopt);
    }
  });
  connect(ui->exit_btn, &QPushButton::clicked, this, []() {
    qDebug() << "exit";
    QApplication::exit(0);
  });

  connect(ui->challenge_btn, &QPushButton::clicked, [this]() {
    auto dialog = new challenge_dialog();
    connect(dialog, &challenge_dialog::ok, this, [this, dialog]() {
      emit enter_challenge_mode(dialog->name);
      dialog->accept();
    });
    connect(dialog, &challenge_dialog::cancle, this,
            [dialog]() { dialog->reject(); });
    dialog->exec();
  });
}

welcome_page::~welcome_page() { delete this->ui; }

main_stack::main_stack(QWidget *parent) : QStackedWidget(parent) {
  this->welcomePage = new welcome_page(this);
  this->addWidget(welcomePage);
  connect(welcomePage, &welcome_page::enter_level_mode, this,
          &main_stack::handle_enter_level_mode);
  connect(welcomePage, &welcome_page::enter_create_mode, this,
          &main_stack::handle_enter_create_mode);
  connect(welcomePage, &welcome_page::enter_challenge_mode, this,
          &main_stack::handle_enter_challenge_mode);
}
void main_stack::return_welcome_page() { this->setCurrentWidget(welcomePage); }
void main_stack::handle_enter_level_mode(const QString &path) {
  auto newLevel = new level_mode(path, this);
  this->addWidget(newLevel);
  this->setCurrentWidget(newLevel);

  connect(newLevel, &level_mode::should_exit, this, [=]() {
    this->removeWidget(newLevel);
    newLevel->deleteLater();
    this->return_welcome_page();
  });
}

void main_stack::handle_enter_create_mode(const std::optional<QString> &path) {
  create_mode *mode;
  if (path.has_value()) {
    mode = new create_mode(path.value(), this);
  } else {
    mode = new create_mode(this);
  }
  this->addWidget(mode);
  this->setCurrentWidget(mode);

  connect(mode, &create_mode::should_exit, this, [=]() {
    this->removeWidget(mode);
    mode->deleteLater();
    this->return_welcome_page();
  });
}
void main_stack::handle_enter_challenge_mode(const QString &name) {
  auto challenge = new challenge_mode(name, this);
  this->addWidget(challenge);
  this->setCurrentWidget(challenge);

  connect(challenge, &challenge_mode::should_exit, this, [=]() {
    this->removeWidget(challenge);
    challenge->deleteLater();
    this->return_welcome_page();
  });
}

main_stack::~main_stack() = default;

#include "welcome_page.moc"