/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 19:11:53
 */
#pragma once
#include "ans_model.hpp"
#include "qtimer.h"
#include <Eigen/Eigen>
#include <Eigen/src/Core/Matrix.h>
#include <qboxlayout.h>
#include <qcontainerfwd.h>
#include <qcoreevent.h>
#include <qdebug.h>
#include <qevent.h>
#include <qgridlayout.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlayoutitem.h>
#include <qlist.h>
#include <qmargins.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qpushbutton.h>
#include <qscopedpointer.h>
#include <qshareddata.h>
#include <qsizepolicy.h>
#include <qsvgrenderer.h>
#include <qtmetamacros.h>
#include <qwidget.h>

namespace treeImage {
inline QImage generateImage(bool plant) {
  if (plant) {
    QSvgRenderer renderer;
    auto image = QImage(300, 300, QImage::Format_RGBA8888);
    image.fill(Qt::transparent);
    renderer.load(QString(":/icon/tree-fill.svg"));
    QPainter p(&image);
    renderer.render(&p);
    return image;
  } else {
    QSvgRenderer renderer;
    auto image = QImage(300, 300, QImage::Format_RGBA8888);
    image.fill(Qt::transparent);
    renderer.load(QString(":/icon/tree-stroke.svg"));
    QPainter p(&image);
    renderer.render(&p);
    return image;
  }
}
static QImage noPlant = generateImage(false);
static QImage plant = generateImage(true);
}; // namespace treeImage

// 负责绘制树是否选中
class tree_item : public QWidget {
private:
  bool isCheck = false;

protected:
  void paintEvent(QPaintEvent *) override {
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    if (this->isCheck) {
      painter.drawImage(this->rect(), treeImage::plant);

    } else {
      painter.drawImage(this->rect(), treeImage::noPlant);
    }
  }

public:
  void resizeEvent(QResizeEvent *event) override {
    auto s = event->size();
    if (s.width() == s.height()) {
      return QWidget::resizeEvent(event);
    }

    auto min = s.height() > s.width() ? s.width() : s.height();
    resize(min, min);
  }
  tree_item(QWidget *parent = nullptr) : QWidget(parent) {}
  void setCheck(bool check) {
    this->isCheck = check;
    this->update();
  }
  bool getCheck() const { return this->isCheck; }
};

class opertor_mat : public QWidget {
protected:
  using mat_ptr = QSharedPointer<bool_table>;

private:
  mat_ptr opmat;
  int id;

protected:
  Eigen::Matrix<tree_item *, 6, 6> btnList;
  void setMat(int i, int j, bool data) { (*this->opmat)(i, j) = data; };
  void setMat(int i, bool data) { (*this->opmat)(i) = data; }

  void paintEvent(QPaintEvent *event) override {
    for (int i = 0; i < opmat->cols(); i++) {
      for (int j = 0; j < opmat->rows(); j++) {
        btnList(j, i)->setCheck((*opmat)(j, i));
      }
    }
    QWidget::paintEvent(event);
  }

public:
  explicit opertor_mat(int id, mat_ptr opmat = mat_ptr(nullptr),
                       QWidget *parent = nullptr) {
    this->opmat = opmat;
    this->id = id;

    auto container = new QWidget(this);
    auto gLayout = new QGridLayout();
    gLayout->setContentsMargins(QMargins(0, 0, 0, 0));
    if (opmat == nullptr) {
      auto content = new bool_table();
      this->opmat = mat_ptr(content);
    }

    for (int i = 0; i < opmat->cols(); i++) {
      for (int j = 0; j < opmat->rows(); j++) {
        auto newBtn = new tree_item(container);
        gLayout->addWidget(newBtn, j, i);
        this->btnList(j, i) = newBtn;
      }
    }
    container->setLayout(gLayout);

    int req = opmat->getRequire();
    auto requireLabel = new QLabel(QString("%1 x %1").arg(req), this);
    requireLabel->setStyleSheet("QLabel{color: black;font-size: 20px}");
    auto labelLayout = new QHBoxLayout();
    labelLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding,
                                               QSizePolicy::Preferred));
    labelLayout->addWidget(requireLabel);
    labelLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding,
                                               QSizePolicy::Preferred));

    auto lay = new QVBoxLayout();
    lay->addWidget(container);
    lay->addLayout(labelLayout);
    lay->setStretch(0, 1);

    this->setLayout(lay);
  }

  int getId() { return this->id; }

  mat_ptr getMat() { return this->opmat; }
  void setMat(mat_ptr p) {
    this->opmat = p;

    this->update();
  }

  int getXNum() { return this->opmat->cols(); }
  int getYNum() { return this->opmat->rows(); }

  mat_ptr getOpMat() { return this->opmat; }
};

class touch_opertor_mat : public opertor_mat {
  Q_OBJECT
private:
  QScopedPointer<QPoint> startPoint = QScopedPointer<QPoint>(nullptr);
  QScopedPointer<QPoint> endPoint = QScopedPointer<QPoint>(nullptr);

protected:
  void paintEvent(QPaintEvent *event) override {
    opertor_mat::paintEvent(event);
    if (!this->startPoint.isNull() && !this->endPoint.isNull()) {
      QRect r = QRect(*this->startPoint, *this->endPoint);
      QPainter painter(this);
      painter.setBrush(Qt::red);
      painter.drawRect(r);
    }
  }
  void mousePressEvent(QMouseEvent *event) override {
    this->startPoint.reset(new QPoint(event->pos()));
    this->endPoint.reset(nullptr);
  }
  void mouseMoveEvent(QMouseEvent *event) override {
    this->endPoint.reset(new QPoint(event->pos()));
    this->update();
  }
  void mouseReleaseEvent(QMouseEvent *event) override {
    if (!startPoint.isNull() && !endPoint.isNull()) {
      QRect selectRect = QRect(*startPoint, *endPoint);
      for (int i = 0; i < btnList.cols(); i++) {
        for (int j = 0; j < btnList.rows(); j++) {
          auto item = btnList(j, i);
          auto p = QPoint(item->pos().x() + item->width() / 2,
                          item->pos().y() + item->height() / 2);
          if (selectRect.contains(p)) {
            this->setMat(j, i, !item->getCheck());
          }
        }
      }
    } else if (!startPoint->isNull()) {
      for (int i = 0; i < btnList.cols(); i++) {
        for (int j = 0; j < btnList.rows(); j++) {
          auto item = btnList(j, i);
          auto rect = item->geometry();
          if (rect.contains(*startPoint)) {
            this->setMat(j, i, !item->getCheck());
          }
        }
      }
    }
    this->startPoint.reset(nullptr);
    this->endPoint.reset(nullptr);
    this->update();
    emit mat_has_update();
  }
signals:
  void mat_has_update();

public:
  explicit touch_opertor_mat(int id, mat_ptr opmat = mat_ptr(nullptr),
                             QWidget *parent = nullptr)
      : opertor_mat(id, opmat, parent) {}
};

class all_page_mat : public opertor_mat {
  Q_OBJECT
private:
  QScopedPointer<QTimer> clickTimer;

protected:
  void mousePressEvent(QMouseEvent *e) override {
    clickTimer.reset(new QTimer());
    clickTimer->setSingleShot(true);
    clickTimer->start(200);
  }
  void mouseReleaseEvent(QMouseEvent *e) override {
    if (clickTimer->isActive()) {
      // 如果定时器还在运行，那么发送一个点击事件
      emit clicked(this->getId());
    }
    clickTimer.reset(nullptr);
  }
signals:
  void clicked(int id);

public:
  explicit all_page_mat(int id, mat_ptr opmat = mat_ptr(nullptr),
                        QWidget *parent = nullptr)
      : opertor_mat(id, opmat, parent) {}
};