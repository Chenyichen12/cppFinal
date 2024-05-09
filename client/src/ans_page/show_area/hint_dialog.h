//
// Created by chen_yichen on 2024/5/9.
//

#pragma once
#include <qdialog.h>
class hint_dialog : public QDialog {
  Q_OBJECT
private:
  struct {
    int row;
    int col;
  } result{1, 1};

public:
  explicit hint_dialog(QWidget *parent = nullptr);
  ~hint_dialog() override;
  std::pair<int, int> get_result() const {
    return {result.row - 1, result.col - 1};
  }

signals:
  void ok();
  void cancel();
};
