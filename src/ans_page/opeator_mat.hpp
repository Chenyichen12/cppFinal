/*
 * @Author: Chenyichen12 sama1538@outlook.com
 * @Date: 2024-04-30 19:11:53
 */
#pragma once
#include <Eigen/Eigen>
#include <qgridlayout.h>
#include <qimage.h>
#include <qlist.h>
#include <qobject.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qshareddata.h>
#include <qsvgrenderer.h>
#include <qwidget.h>

namespace treeImage {
inline QImage generateImage(bool plant) {
  if (plant) {
    QSvgRenderer renderer;
    auto image = QImage(300, 300, QImage::Format_RGBA8888);
    memset(image.bits(), 0x00, image.sizeInBytes());
    renderer.load(QString(":/icon/tree-fill.svg"));
    QPainter p(&image);
    renderer.render(&p);
    return image;
  } else {
    QSvgRenderer renderer;
    auto image = QImage(300, 300, QImage::Format_RGBA8888);
    memset(image.bits(), 0x00, image.sizeInBytes());
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
class tree_item : public QPushButton {
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
  tree_item(QWidget *parent = nullptr) : QPushButton(parent) {}
  void setCheck(bool check) {
    this->isCheck = check;
    this->update();
  }
};

class opertor_mat : public QWidget {
  using mat_ptr = QSharedPointer<Eigen::Matrix<bool, 6, 6>>;
  Q_OBJECT
private:
  mat_ptr opmat;
  int id;
  QList<tree_item *> btnList;

public:
  explicit opertor_mat(int id, mat_ptr opmat = mat_ptr(nullptr),
                       QWidget *parent = nullptr) {
    this->opmat = opmat;
    this->id = id;

    this->btnList = QList<tree_item *>(36);
    auto gLayout = new QGridLayout();
    if (opmat != nullptr) {
      auto content = new Eigen::Matrix<bool, 6, 6>();
      content->setConstant(false);
      this->opmat = mat_ptr(content);
    }
    for (int i = 0; i < opmat->cols(); i++) {
      for (int j = 0; j < opmat->rows(); j++) {
        auto newBtn = new tree_item(this);
        newBtn->setFixedSize(40, 40);
        gLayout->addWidget(newBtn, j, i);
        newBtn->setCheck((*opmat)(j, i));
      }
    }
    this->setLayout(gLayout);
  }

  int getId() { return this->id; }

  mat_ptr getMat() { return this->opmat; }
  void setMat(mat_ptr p) {
    this->opmat = p;

    this->update();
  }

  int getXNum() { return this->opmat->cols(); }
  int getYNum() { return this->opmat->rows(); }
};
