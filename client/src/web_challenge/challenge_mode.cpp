//
// Created by chen_yichen on 2024/5/12.
//

#include "challenge_mode.h"
#include "../decomposer/decompose_mat.h"
#include "ans_page/ans_stack.hpp"
#include "ans_page/show_area/show_widget.hpp"
#include <QGridLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QScrollArea>
class show_question_mat : public show_widget {
  Q_OBJECT
public:
  enum hardness { easy = 0, normal = 1 };

private:
  hardness hardNess;
  QImage getImageFromNumber(int num) override {
    if (this->hardNess == 0) {
      return show_widget::getImageFromNumber(num);
    } else {
      if (num == 0) {
        return two_tree;
      } else {
        return show_widget::getImageFromNumber(num);
      }
    }
  }

protected:
  void mousePressEvent(QMouseEvent *event) override { emit click(); }

public:
  explicit show_question_mat(const QList<int> &list, hardness hardNess = easy,
                             QWidget *parent = nullptr)
      : show_widget(list, parent), hardNess(hardNess) {}
  void setHardness(hardness h) {
    this->hardNess = h;
    this->update();
  }
signals:
  void click();
};

class challenge_mode::waitPage : public QWidget {
  QList<QLabel *> users;
  QGridLayout *layout;

public:
  explicit waitPage(QWidget *parent = nullptr) : QWidget(parent) {
    layout = new QGridLayout(this);
    auto title = new QLabel("Waiting for other players to join", this);
    layout->addWidget(title, 0, 0, 1, 3);
  }
  void add_users(const QString &m_name) {
    auto new_user = new QLabel(m_name, this);
    QFont f = QFont();
    f.setPointSize(10);
    new_user->setFont(f);
    users.push_back(new_user);
    int x = (int)(users.size() - 1) / 3 + 1;
    int y = (int)(users.size() - 1) % 3;
    layout->addWidget(new_user, x, y);
  }
  void clearUser() {
    for (auto user : users) {
      layout->removeWidget(user);
      user->deleteLater();
    }
    users.clear();
  }
};

class challenge_mode::introPage : public QScrollArea {
  Q_OBJECT
  QGridLayout *layout;
  QList<show_question_mat *> questions_widget;
  QList<bool> completed;

public:
  explicit introPage(const QList<Eigen::Matrix<int, 6, 6>> &questions,
                     QWidget *parent = nullptr)
      : QScrollArea(parent) {
    layout = new QGridLayout();

    auto content = new QWidget(this);
    auto title = new QLabel("Challenge Mode", this);
    content->setLayout(layout);
    this->setWidget(content);
    this->setAutoFillBackground(true);
    this->setWidgetResizable(true);

    layout->addWidget(title, 0, 0, 1, 3);
    for (int i = 0; i < questions.size(); i++) {
      auto list = QList<int>();
      for (int j = 0; j < questions[i].size(); j++) {
        list.append(questions[i](j));
      }

      auto question =
          new show_question_mat(list, show_question_mat::normal, this);

      connect(question, &show_question_mat::click, this, [this]() {
        auto sender = qobject_cast<show_question_mat *>(QObject::sender());
        int index = (int)questions_widget.indexOf(sender);
        if (index == -1) {
          return;
        }
        emit click_question(index);
      });

      questions_widget.append(question);
      completed.append(false);

      auto row = i / 3 + 1;
      auto col = i % 3;
      layout->addWidget(question, row, col);
      question->setMinimumSize(QSize(500, 500));
    }
  }

  void setCompleted(int index) {
    if (index < 0 || index >= completed.size()) {
      return;
    }
    completed[index] = true;
  }

  bool isAllComplete() {
    for (const auto &item : this->completed) {
      if (item == false) {
        return false;
      }
    }
    return true;
  }

  bool isComplete(int index) { return completed[index]; }

  int getQuestionSize() const { return (int)questions_widget.size(); }
  void setQuestionHardness(int index, show_question_mat::hardness h) {
    if (index < 0 || index >= questions_widget.size()) {
      return;
    }
    questions_widget[index]->setHardness(h);
  }

signals:
  void click_question(int index);
};

QWidget *connect_page(QWidget *w) {
  auto connectPage = new QWidget(w);
  auto layout = new QVBoxLayout();
  auto label = new QLabel("Connecting to server", connectPage);
  auto QFont = label->font();
  QFont.setPointSize(30);
  label->setFont(QFont);
  layout->addWidget(label);
  connectPage->setLayout(layout);
  return connectPage;
}

class ui_challenge_game_widow {
public:
  QWidget *ansArea;
  QWidget *showArea;
  QWidget *sideBar;
  QPushButton *hintBtn;
  QPushButton *display_score_btn;
  void setupUi(QWidget *w) {
    this->showArea = new QWidget(w);
#if uiTest
    showArea->setObjectName("showArea");
    showArea->setStyleSheet("QWidget#showArea{background-color:white;}");
#endif
    showArea->setMinimumSize(600, 400);
    auto layoutTop = new QHBoxLayout;
    layoutTop->addSpacerItem(
        new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layoutTop->addWidget(showArea);
    sideBar = new QWidget(w);
    hintBtn = new QPushButton("提示....", w);
    display_score_btn = new QPushButton("显示排行榜", w);

    auto sideBarLayout = new QVBoxLayout();
    sideBarLayout->addWidget(hintBtn);
    sideBarLayout->addWidget(display_score_btn);
    sideBar->setLayout(sideBarLayout);
    layoutTop->addWidget(sideBar);

    layoutTop->addSpacerItem(
        new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));

    ansArea = new QWidget(w);
#if uiTest
    ansArea->setObjectName("ansArea");
    ansArea->setStyleSheet("QWidget#ansArea{background-color: white;}");
#endif

    auto mainLayout = new QVBoxLayout();
    mainLayout->addLayout(layoutTop);
    mainLayout->addWidget(ansArea);
    w->setBaseSize(1000, 800);
    w->resize(1000, 800);
    w->setLayout(mainLayout);
  }
};

class all_user_board : public QDialog {
private:
  QList<user_name_score> d;

public:
  explicit all_user_board(const QList<user_name_score> &datas,
                          QWidget *parent = nullptr)
      : QDialog(parent) {
    this->d = datas;
    auto get_an_item = [this](const user_name_score &dd) {
      auto newLayout = new QHBoxLayout();
      auto nameLabel = new QLabel(this);
      nameLabel->setText(dd.name);
      auto scoreLabel = new QLabel(this);
      scoreLabel->setText(QString("%1").arg(dd.score));
      newLayout->addWidget(nameLabel);
      newLayout->addWidget(scoreLabel);
      return newLayout;
    };

    auto mainLayout = new QVBoxLayout();
    for (const auto &item : d) {
      auto l = get_an_item(item);
      mainLayout->addLayout(l);
    }
    this->setLayout(mainLayout);
    this->setAttribute(Qt::WA_DeleteOnClose);
  }
};

class challenge_game_window : public QWidget {
  Q_OBJECT
private:
  ui_challenge_game_widow *ui;
  ans_stack *ans_area;
  show_question_mat *question_mat;

public:
  explicit challenge_game_window(const QList<int> &mats,
                                 QWidget *parent = nullptr)
      : QWidget(parent) {
    ui = new ui_challenge_game_widow();
    ui->setupUi(this);

    question_mat = new show_question_mat(mats, show_question_mat::easy, this);
    auto layout = new QVBoxLayout();
    layout->addWidget(question_mat);
    this->ui->showArea->setLayout(layout);

    this->ans_area = new ans_stack(ui->ansArea);
    auto ansL = new QVBoxLayout();
    ansL->addWidget(ans_area);
    this->ui->ansArea->setLayout(ansL);

    connect(ui->hintBtn, &QPushButton::clicked, [this]() {
      auto ans = this->question_mat->borrowMat();
      auto d = decomposer(*ans);
      d.Decompose();
      auto res = d.get_ans_mat();
      auto model = this->ans_area->borrowModel()->getMat();
      for (int i = 0; i < model.size(); i++) {
        auto mat = model[i];
        auto anss = res[i];
        mat->setConstant(false);
        for (int j = 0; j < anss.size(); j++) {
          (*mat)(j) = anss(j) == 1;
        }
      }

      ans_area->update();
    });
    connect(ui->display_score_btn, &QPushButton::clicked, [this]() {
      auto user_data = emit require_user_score();
      for (const auto &item : user_data) {
        qDebug() << item.name << item.score;
      }
      auto dialog = new all_user_board(user_data, this);
      dialog->setModal(true);
      dialog->show();
    });
    connect(ans_area, &ans_stack::submit_to_window, [this](ans_model *model) {
      auto mat = this->question_mat->borrowMat();
      emit submit(model, mat);
    });
  }
  ~challenge_game_window() override { delete ui; }

  void setHardness(show_question_mat::hardness h) {
    this->question_mat->setHardness(h);
  }
  static bool checkCorrect(ans_model *borrow_model, show_mat *borrow_mat) {
    auto matList = borrow_model->getMat();
    for (const auto &booltable : matList) {
      if (!booltable->ifComplete()) {
        return false;
      }
    }
    auto cols = matList[0]->cols();
    auto rows = matList[0]->rows();

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        int count = 0;
        for (const auto &booltable : matList) {
          if ((*booltable)(i, j))
            count++;
        }

        if (count != (*borrow_mat)(i, j)) {
          return false;
        }
      }
    }
    return true;
  }
signals:
  void submit(ans_model *borrowModel, show_mat *borrowAns);
  QList<user_name_score> require_user_score();
};

challenge_mode::challenge_mode(const QString &name, QWidget *parent)
    : QStackedWidget(parent) {
  this->name = name;
  connectPage = connect_page(this);
  this->intro = nullptr;
  this->addWidget(connectPage);
  this->setCurrentWidget(connectPage);
  this->state = connecting;

  this->wait = new challenge_mode::waitPage(this);
  wait->add_users(name);
  this->addWidget(wait);
  socket = new QWebSocket("client", QWebSocketProtocol::VersionLatest, this);
  connect(socket, &QWebSocket::connected, this, [this]() {
    auto obj = QJsonObject();
    obj.insert("type", 0);
    obj.insert("name", this->name);
    QJsonDocument doc;
    doc.setObject(obj);
    socket->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    this->setCurrentWidget(this->wait);
    this->state = waiting;
    //    qDebug() << "waiting";
  });

  connect(socket, &QWebSocket::textMessageReceived, this,
          [this](const QString &s) {
            this->handle_web_event(s);
            // qdebug()
          });

  socket->open(QUrl("ws://localhost:10101"));
}
void challenge_mode::handle_web_event(const QString &event) {
  auto doc = QJsonDocument::fromJson(event.toUtf8());
  auto obj = doc.object();
  auto type = obj["type"].toInt();
  switch (type) {
  case 0:
    qDebug() << "uuid_ack";
    this->uuid = obj["uuid"].toString();
    break;
  case 1: {
    auto info = obj["users"].toArray();
    if (state == waiting) {
      this->wait->clearUser();
      for (auto &&i : info) {
        auto o = i.toObject();
        this->wait->add_users(o["name"].toString());
      }
    }
    QList<user_name_score> use;

    for (auto &&i : info) {
      auto o = i.toObject();
      auto n = o["name"].toString();
      auto score = o["score"].toInt();
      use.append({n, score});
    }
    this->users = use;
    break;
  }
  case 2:
    if (state == waiting) {
      auto questions = obj.value("questions").toArray();
      this->state = intro_page;
      QList<Eigen::Matrix<int, 6, 6>> questionList;
      for (auto &&question : questions) {
        auto q = question.toArray();
        Eigen::Matrix<int, 6, 6> mat;
        for (int j = 0; j < q.size(); ++j) {
          mat(j) = q[j].toInt();
        }
        questionList.append(mat);
      }
      this->intro = new introPage(questionList, this);
      for (int i = 0; i < questionList.size(); ++i) {
        QList<int> list;
        for (int j = 0; j < questionList[i].size(); ++j) {
          list.append(questionList[i](j));
        }
        auto game_window = new challenge_game_window(list, this);
        this->addWidget(game_window);
        this->game_pages.append(game_window);
        connect(game_window, &challenge_game_window::require_user_score, this,
                &challenge_mode::handle_require_score);
        connect(game_window, &challenge_game_window::submit, this,
                [this, i](ans_model *model, show_mat *mat) {
                  auto res = challenge_game_window::checkCorrect(model, mat);
                  if (res) {

                    auto msgBox = new QMessageBox(this);
                    msgBox->setAttribute(Qt::WA_DeleteOnClose);
                    msgBox->setText("作答正确");

                    msgBox->addButton("返回", QMessageBox::AcceptRole);
                    msgBox->exec();
                    this->intro->setCompleted(i);
                    this->setCurrentWidget(this->intro);
                    auto ScoreJson = QJsonDocument();
                    auto obj = QJsonObject();
                    obj.insert("type", i < 15 ? 1 : 2);
                    ScoreJson.setObject(obj);
                    socket->sendTextMessage(
                        ScoreJson.toJson(QJsonDocument::Compact));
                    bool ifAllComplete = this->intro->isAllComplete();
                    if (ifAllComplete) {
                      emit this->should_exit();
                    }

                  } else {
                    // 作答错误
                    auto msgBox = new QMessageBox(
                        QMessageBox::Icon::Critical, "错误的答案", "作答错误",
                        QMessageBox::StandardButton::NoButton, this);
                    msgBox->setAttribute(Qt::WA_DeleteOnClose);
                    msgBox->setWindowFlags(Qt::Window | Qt::WindowTitleHint |
                                           Qt::CustomizeWindowHint);
                    auto retryBtn = new QPushButton("重试(10s)", msgBox);
                    retryBtn->setEnabled(false);
                    msgBox->addButton(retryBtn, QMessageBox::AcceptRole);
                    auto *timer = new QTimer(msgBox);
                    int *count = new int(10);
                    connect(timer, &QTimer::timeout, this,
                            [retryBtn, count, timer]() {
                              (*count)--;
                              if (*count == 0) {
                                retryBtn->setText(QString("重试"));
                                delete count;
                                timer->stop();
                                retryBtn->setEnabled(true);
                              } else {
                                retryBtn->setText(
                                    QString("重试%1s").arg(*count));
                              }
                            });
                    timer->start(1000);
                    msgBox->exec();
                  }
                });

        if (i >= 15) {
          this->intro->setQuestionHardness(i,
                                           show_question_mat::hardness::normal);
          game_window->setHardness(show_question_mat::hardness::normal);
        } else {
          this->intro->setQuestionHardness(i,
                                           show_question_mat::hardness::easy);
          game_window->setHardness(show_question_mat::hardness::easy);
        }
      }
      connect(intro, &introPage::click_question, this, [this](int i) {
        if (this->intro->isComplete(i)) {
          auto msgBox = new QMessageBox(this);
          msgBox->setAttribute(Qt::WA_DeleteOnClose);
          msgBox->setText("已经完成");
          msgBox->addButton("返回", QMessageBox::AcceptRole);
          msgBox->exec();
          return;
        }
        this->setCurrentWidget(this->game_pages[i]);
        this->state = make_game;
      });
      this->addWidget(intro);
      this->setCurrentWidget(intro);
    }
  default:
    break;
  }
}
QList<user_name_score> challenge_mode::handle_require_score() {
  return this->users;
}

// #ifdef TREE_TEST
// #include <QApplication>
// #include <QTimer>
// #include <gtest/gtest.h>
// TEST(challenge_mode, show_question_mat) {
//   auto c = new challenge_mode("test");
//   c->show();
//   QApplication::exec();
// }
// #endif

#include "challenge_mode.moc"