//
// Created by chen_yichen on 2024/5/10.
//
#include "web_socket.h"
#include <iostream>
#include <qapplication.h>
#include <qthread.h>
#include <qwidget.h>

enum command { HELP, SHOW_USERS, START_GAME, EXIT };

QHash<QString, command> command_map = {
    {"help", HELP},
    {"show_users", SHOW_USERS},
    {"start_game", START_GAME},
    {"exit", EXIT},
};
class cmd_thread : public QObject {
  Q_OBJECT
private:
  void wait_input() {
    std::string str;
    std::cin >> str;
    this->dispatch_command(QString::fromStdString(str));
  }

  void dispatch_command(const QString &str) {
    do {
      auto com = command_map.find(str);
      if (com == command_map.end()) {
        std::cout << "command not found" << std::endl;
        break;
      }

      switch (com.value()) {
      case HELP:
        std::cout << "help" << std::endl;
        break;
      case SHOW_USERS:
        emit show_users();
        break;
      case START_GAME:
        std::cout << "start_game" << std::endl;
        break;
      case EXIT:
        emit exit();
        break;
      }
    } while (false);
    this->next_command();
  }
  void next_command() { this->wait_input(); }

public:
  explicit cmd_thread(const net_socket *scoket) {
    //    connect(this, &cmd_thread::exit, qApp, &QApplication::quit);
    connect(this, &cmd_thread::exit, scoket, []() { QApplication::quit(); });
    connect(this, &cmd_thread::show_users, scoket, &net_socket::show_all_users);
  }
public slots:
  void run() { this->wait_input(); }
signals:
  void show_users();
  void exit();
};
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  auto socket = new net_socket();
  auto read_thread = new QThread();
  auto cmd = new cmd_thread(socket);
  QObject::connect(read_thread, &QThread::started, cmd, &cmd_thread::run);
  cmd->moveToThread(read_thread);

  read_thread->start();
  return QApplication::exec();
}
#include "main.moc"