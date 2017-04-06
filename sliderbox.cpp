#include "sliderbox.h"

sliderbox::sliderbox(QWidget *parent) : QCheckBox(parent)
{
    setCheckState(Qt::CheckState::Checked);
}

void sliderbox::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.fillRect(rect(), Qt::lightGray);
  QRect switchRect;
  QColor switchColor;
  QString text;

  int halfWidth = width() / 2;

  if (isChecked())
  {
    switchRect = QRect(halfWidth, 0, halfWidth, height());
    switchColor = QColor(Qt::gray);
    text = "On";
  }
  else
  {
    switchRect = QRect(0, 0, halfWidth, height());
    switchColor = QColor(Qt::darkGray);
    text = "Off";
  }
  painter.fillRect(switchRect, switchColor);
  painter.drawText(switchRect, Qt::AlignCenter, text);
}
