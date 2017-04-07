#ifndef SLIDERBOX_H
#define SLIDERBOX_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>
#include <QObject>

class sliderbox : public QPushButton
{
  Q_OBJECT

public:
  sliderbox(QWidget *parent);

  void setDisabled(bool value);
  bool isDisabled() const;

protected:
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
  QSize sizeHint() const Q_DECL_OVERRIDE;
  QSize minimumSizeHint() const Q_DECL_OVERRIDE;
  void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
  void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
  void enterEvent(QEvent *) Q_DECL_OVERRIDE;

private:
  qint16 _height;
  qint16 _thumbsize;
  QColor _brush;
  QColor _thumb;
  QColor _thumbborder;

signals:
  void toggled();
};

#endif // SLIDERBOX_H
