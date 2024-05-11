//
// Created by chen_yichen on 2024/5/11.
//

#pragma once
#include <QDateTime>
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

public:
  explicit game_users(QObject *parent = nullptr);
  explicit game_users(const QString &name, QObject *parent = nullptr);

  void set_name(const QString &m_name);
  void start_game();
  void set_score(int s);
  int get_score() const;
  const QString &getId();
  std::optional<qint64> during() const;
  std::optional<qint64> during(const QDateTime &time) const;
};
