//
// Created by chen_yichen on 2024/5/10.
//

#pragma once
#include "game_users.h"
#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>

class net_socket : public QObject {
  Q_OBJECT
public:
  explicit net_socket(QObject *parent = nullptr);
  ~net_socket() override;

public slots:
  void show_all_users();
  void handle_start_game();
  void submit_score_to_client(game_users* user);

private:
  QWebSocketServer *server;
  QList<game_users *> clients;
  QJsonObject generateUsersInformation();
private slots:
  void on_new_connected();
};
