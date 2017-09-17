#include "ui_sensors.h"
#include "ui_ui_sensors.h"

ui_sensors::ui_sensors(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_sensors)
{
    ui->setupUi(this);
}

ui_sensors::~ui_sensors()
{
    delete ui;
}
