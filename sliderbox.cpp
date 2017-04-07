#include "sliderbox.h"

sliderbox::sliderbox(QWidget *parent) : QAbstractButton(parent),
                                        _height(10),
                                        _thumbsize(5),
                                        _disabled(false),
                                        _state(false),
                                        _brush("#009688"),
                                        _thumb("#E0E0E0"),
                                        _thumbborder("#888888")
{
    setCheckable(true);
}

bool sliderbox::isChecked() const
{
  return _state;
}

void sliderbox::setState(bool value)
{
  _state = value;
  repaint();
}

void sliderbox::setDisabled(bool value)
{
  _disabled = value;
}

bool sliderbox::isDisabled() const
{
  return _disabled;
}

void sliderbox::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setRenderHint(QPainter::Antialiasing);

  QPainterPath _primary;
  qint16 _x;

  if (_state) {
      painter.setBrush(_brush);
      painter.setOpacity(0.500);
      _x = width() - _height - 2*_thumbsize;
  }
  else
  {
      painter.setBrush(QColor("#000000"));
      painter.setOpacity(0.380);
      _x = _height / 2;
  }

  qreal _radius = _height / 2;
  _primary.addRoundedRect(QRect(_thumbsize, _thumbsize, width() - 2 * _thumbsize,
                                height() - 2 * _thumbsize), _radius, _radius);
  painter.drawPath(_primary.simplified());

  painter.setOpacity(1.0);

  painter.setBrush(_thumbborder);
  painter.drawEllipse(QRect(_x - (_height / 2), 0,
                            height(), height()));
  painter.setBrush(_thumb);

  painter.drawEllipse(QRectF(_x - (_height / 2)+1, 1,
                             height()-2, height()-2));

}

QSize sliderbox::sizeHint() const
{
    return minimumSizeHint();
}

QSize sliderbox::minimumSizeHint() const
{
    return QSize(2 * (_height + _thumbsize), _height + 2 * _thumbsize);
}

void sliderbox::mousePressEvent(QMouseEvent *e)
{
  if (!_disabled)
  {
    if (e->buttons() & Qt::LeftButton)
    {
      e->accept();
    }
    else
    {
      e->ignore();
    }
  }
}

void sliderbox::mouseReleaseEvent(QMouseEvent *e)
{
    if (!_disabled)
    {
        if (e->type() == QMouseEvent::MouseButtonRelease && e->button() == Qt::LeftButton)
        {
            e->accept();
            _state = !_state;
            emit toggled();
        }
        else
        {
            e->ignore();
        }
    }
    repaint();
}

void sliderbox::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void sliderbox::enterEvent(QEvent *)
{
  if (_disabled)
    setCursor(Qt::ArrowCursor);
  else
    setCursor(Qt::PointingHandCursor);
}
