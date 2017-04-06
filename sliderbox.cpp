#include "sliderbox.h"

sliderbox::sliderbox(QWidget *parent) : QCheckBox(parent)
{
    setCheckState(Qt::CheckState::Checked);
}
