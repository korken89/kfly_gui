#include "ui_rc_input_channel.h"
#include "ui_ui_rc_input_channel.h"

ui_rc_input_channel::ui_rc_input_channel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_rc_input_channel)
{
    ui->setupUi(this);

    ui->boxRole->addItem("Disabled");
    ui->boxRole->addItem("Throttle");
    ui->boxRole->addItem("Pitch");
    ui->boxRole->addItem("Roll");
    ui->boxRole->addItem("Yaw");
    ui->boxRole->addItem("Aux 1");
    ui->boxRole->addItem("Aux 2");
    ui->boxRole->addItem("Aux 3");
    ui->boxRole->addItem("Flight Mode");
    ui->boxRole->addItem("Arming Switch");
    ui->boxRole->addItem("Serial Control");

    ui->boxType->addItem("Analog");
    ui->boxType->addItem("2-state Switch");
    ui->boxType->addItem("3-state Switch");
}

ui_rc_input_channel::~ui_rc_input_channel()
{
    delete ui;
}

void ui_rc_input_channel::set_channel_number(int i)
{
    ui->labelChannel->setText(QString::number(i));
}


int ui_rc_input_channel::get_minimum()
{
    return ui->spinBottom->value();
}

int ui_rc_input_channel::get_center()
{
    return ui->spinCenter->value();
}

int ui_rc_input_channel::get_maximum()
{
    return ui->spinTop->value();
}

void ui_rc_input_channel::set_minimum(int value)
{
    ui->spinBottom->setValue(value);
}

void ui_rc_input_channel::set_center(int value)
{
    ui->spinCenter->setValue(value);
}

void ui_rc_input_channel::set_maximum(int value)
{
    ui->spinTop->setValue(value);
}




void ui_rc_input_channel::on_boxRole_currentTextChanged(const QString &text)
{

    if (text == "Disabled")
    {
        ui->barRCValue->setValue(-100);
        ui->spinBottom->setEnabled(false);
        ui->spinCenter->setEnabled(false);
        ui->spinTop->setEnabled(false);
        ui->buttonInvert->setEnabled(false);
        ui->boxType->setEnabled(false);
    }
    else if (text == "Throttle" || text == "Pitch" ||
             text == "Roll" || text == "Yaw")
    {
        ui->boxType->setCurrentIndex(0);
        ui->boxType->setEnabled(false);
        ui->spinBottom->setEnabled(true);
        ui->spinCenter->setEnabled(true);
        ui->spinTop->setEnabled(true);
        ui->buttonInvert->setEnabled(true);
    }
    else if (text == "Arming Switch" || text == "Serial Control")
    {
        ui->boxType->setCurrentIndex(1);
        ui->boxType->setEnabled(false);
    }
    else
    {
        ui->boxType->setEnabled(true);
        ui->spinBottom->setEnabled(true);
        ui->spinCenter->setEnabled(true);
        ui->spinTop->setEnabled(true);
        ui->buttonInvert->setEnabled(true);
    }

    emit value_changed();
}

void ui_rc_input_channel::on_spinBottom_valueChanged(int value)
{
    if (value > ui->spinCenter->value())
        ui->spinBottom->setValue(ui->spinCenter->value());

    emit value_changed();
}

void ui_rc_input_channel::on_spinCenter_valueChanged(int value)
{
    if (value < ui->spinBottom->value())
        ui->spinCenter->setValue(ui->spinBottom->value());
    else if (value > ui->spinTop->value())
        ui->spinCenter->setValue(ui->spinTop->value());

    emit value_changed();
}

void ui_rc_input_channel::on_spinTop_valueChanged(int value)
{
    if (value < ui->spinCenter->value())
        ui->spinTop->setValue(ui->spinCenter->value());

    emit value_changed();
}

void ui_rc_input_channel::on_boxType_currentTextChanged(const QString &)
{
    emit value_changed();
}

void ui_rc_input_channel::on_buttonInvert_toggled(bool)
{
    emit value_changed();
}
