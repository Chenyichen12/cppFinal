//
// Created by chen_yichen on 2024/5/10.
//

#pragma once
#include <QObject>
#include <QTcpServer>
class net_socket : public QObject {
  Q_OBJECT
public:
  explicit net_socket(QObject *parent = nullptr);
  ~net_socket() override;
signals:
  void user_add_request(const QString &name);

private:
  QTcpServer *server;
  QList<QTcpSocket *> clients;

private slots:
  void on_new_connected();
  void on_socket_read();
};
