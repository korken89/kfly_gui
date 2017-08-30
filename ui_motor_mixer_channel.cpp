#include "ui_motor_mixer_channel.h"
#include "ui_ui_motor_mixer_channel.h"

ui_motor_mixer_channel::ui_motor_mixer_channel(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ui_motor_mixer_channel)
{
  ui->setupUi(this);
}

ui_motor_mixer_channel::~ui_motor_mixer_channel()
{
  delete ui;
}

void ui_motor_mixer_channel::set_channel_number(int i)
{
    ui->channelLabel->setText(QString::number(i));
}

float ui_motor_mixer_channel::get_roll_weight()
{
    return ui->rollSpinBox->value();
}

void ui_motor_mixer_channel::set_roll_weight(float value)
{
    ui->rollSpinBox->setValue(value);
}

float ui_motor_mixer_channel::get_pitch_weight()
{
    return ui->pitchSpinBox->value();
}

void ui_motor_mixer_channel::set_pitch_weight(float value)
{
    ui->pitchSpinBox->setValue(value);
}

float ui_motor_mixer_channel::get_yaw_weight()
{
    return ui->yawSpinBox->value();
}

void ui_motor_mixer_channel::set_yaw_weight(float value)
{
    ui->yawSpinBox->setValue(value);
}

float ui_motor_mixer_channel::get_throttle_weight()
{
    return ui->throttleSpinBox->value();
}

void ui_motor_mixer_channel::set_throttle_weight(float value)
{
    ui->throttleSpinBox->setValue(value);
}

float ui_motor_mixer_channel::get_offset()
{
    return ui->offsetSpinBox->value();
}

void ui_motor_mixer_channel::set_offset(float value)
{
    ui->offsetSpinBox->setValue(value);
}


void ui_motor_mixer_channel::on_rollSpinBox_valueChanged(double)
{
    emit value_changed();
}

void ui_motor_mixer_channel::on_pitchSpinBox_valueChanged(double)
{
    emit value_changed();
}

void ui_motor_mixer_channel::on_yawSpinBox_valueChanged(double)
{
    emit value_changed();
}

void ui_motor_mixer_channel::on_throttleSpinBox_valueChanged(double)
{
    emit value_changed();
}

void ui_motor_mixer_channel::on_offsetSpinBox_valueChanged(double)
{
    emit value_changed();
}
