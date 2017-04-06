#ifndef SLIDERBOX_H
#define SLIDERBOX_H

#include <QWidget>
#include <QCheckBox>
#include <QPainter>

class sliderbox : public QCheckBox
{
public:
    sliderbox(QWidget *parent);
    void paintEvent(QPaintEvent *event) override;
};

#endif // SLIDERBOX_H
