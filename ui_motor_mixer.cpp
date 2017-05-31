#include "ui_motor_mixer.h"
#include "ui_ui_motor_mixer.h"

ui_motor_mixer::ui_motor_mixer(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ui_motor_mixer)
{
  ui->setupUi(this);
}

ui_motor_mixer::~ui_motor_mixer()
{
  delete ui;
}
