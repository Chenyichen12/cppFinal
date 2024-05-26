//
// Created by chen_yichen on 2024/5/12.
//

#pragma once
#include <Eigen/Core>
#include <QStackedWidget>
#include <QWebSocket>
class challenge_game_window;
class challenge_mode : public QStackedWidget {
  Q_OBJECT
  enum game_state { waiting = 0, intro_page = 1, make_game = 2, connecting };

private:
  class waitPage;
  class introPage;

  game_state state;
  challenge_mode::waitPage *wait;

  challenge_mode::introPage *intro;
  QList<challenge_game_window *> game_pages;

  QWidget *connectPage;
  QWebSocket *socket;
  QString name;
  QString uuid;

public:
  explicit challenge_mode(const QString &name, QWidget *parent = nullptr);
  void handle_web_event(const QString &event);
};
