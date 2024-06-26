//
// Created by chen_yichen on 2024/5/11.
//

#pragma once
#include "scoket_event_type.h"
#include <QDateTime>
#include <QWebSocket>
#include <qobject.h>
#include <qtimer.h>
#include <quuid.h>
class game_users : public QObject {
  Q_OBJECT
protected:
  QString name;
  int score;
  std::optional<QDateTime> start_time;
  QString uuid;

  QWebSocket *tcpSocket;

public:
  explicit game_users(QWebSocket *socket, QObject *parent = nullptr);

  void set_name(const QString &m_name);
  void start_game();
  void set_score(int s);
  int get_score() const;
  const QString &getId();
  std::optional<qint64> during() const;
  std::optional<qint64> during(const QDateTime &time) const;
  const QString &getName();

  virtual void handle_socket_event(const QByteArray &array);
  void ackUUID();
  user_information getInformation();

  void send_message(const QByteArray &message);
signals:
  void user_name_change(const QString &name);
  void user_score_change(int score);
  void user_init_complete();
  void user_acquire_score();
};
