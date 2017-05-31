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
