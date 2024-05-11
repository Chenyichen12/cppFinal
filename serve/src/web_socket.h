//
// Created by chen_yichen on 2024/5/10.
//

#pragma once
#include "game_users.h"
#include <QObject>
#include <QTcpServer>
class net_socket : public QObject {
  Q_OBJECT
public:
  explicit net_socket(QObject *parent = nullptr);
  ~net_socket() override;

private:
  QTcpServer *server;
  QList<game_users *> clients;

private slots:
  void on_new_connected();
};
