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

void ui_rc_input_channel::set_current_value(float cal_value, int raw_value)
{
    if (ui->boxType->currentText() == "2-state Switch")
    {
        cal_value = ((get_maximum() + get_minimum())/2 > raw_value) ? -1 : 1;
    }
    else if (ui->boxType->currentText() == "3-state Switch")
    {
        int diff = (get_maximum() - get_minimum())/3;

        if (raw_value <= get_minimum()+diff)
            cal_value = -1;
        else if (raw_value > get_minimum()+diff && raw_value <= get_maximum()-diff)
            cal_value = 0;
        else
            cal_value = 1;
    }

    ui->barRCValue->setValue(static_cast<int>(100*cal_value));
    ui->barRCValue->setFormat(QString::number(static_cast<int>(100*cal_value)) +
                              "% (" + QString::number(raw_value) +
                              ")");
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



kfly_comm::enums::RCInput_Role ui_rc_input_channel::get_role()
{
    if (ui->boxRole->currentText() == "Throttle")
        return kfly_comm::enums::RCInput_Role::ROLE_THROTTLE;

    else if (ui->boxRole->currentText() == "Pitch")
        return kfly_comm::enums::RCInput_Role::ROLE_PITCH;

    else if (ui->boxRole->currentText() == "Roll")
        return kfly_comm::enums::RCInput_Role::ROLE_ROLL;

    else if (ui->boxRole->currentText() == "Yaw")
        return kfly_comm::enums::RCInput_Role::ROLE_YAW;

    else if (ui->boxRole->currentText() == "Aux 1")
        return kfly_comm::enums::RCInput_Role::ROLE_AUX1;

    else if (ui->boxRole->currentText() == "Aux 2")
        return kfly_comm::enums::RCInput_Role::ROLE_AUX2;

    else if (ui->boxRole->currentText() == "Aux 3")
        return kfly_comm::enums::RCInput_Role::ROLE_AUX3;

    else if (ui->boxRole->currentText() == "Flight Mode")
        return kfly_comm::enums::RCInput_Role::ROLE_FLIGHTMODE;

    else if (ui->boxRole->currentText() == "Arming Switch")
        return kfly_comm::enums::RCInput_Role::ROLE_ARM_NONLATCH;

    else if (ui->boxRole->currentText() == "Serial Control")
        return kfly_comm::enums::RCInput_Role::ROLE_ENABLE_SERIAL_CONTROL;

    else
        return kfly_comm::enums::RCInput_Role::ROLE_OFF;

}

kfly_comm::enums::RCInput_Type ui_rc_input_channel::get_type()
{
    if (ui->boxType->currentText() == "2-state Switch")
        return kfly_comm::enums::RCInput_Type::TYPE_ON_OFF;

    else if (ui->boxType->currentText() == "3-state Switch")
        return kfly_comm::enums::RCInput_Type::TYPE_3_STATE;

    else
        return kfly_comm::enums::RCInput_Type::TYPE_ANALOG;
}

bool ui_rc_input_channel::get_inverted()
{
    return ui->buttonInvert->isChecked();
}

void ui_rc_input_channel::set_role(kfly_comm::enums::RCInput_Role role)
{
    if (role == kfly_comm::enums::RCInput_Role::ROLE_THROTTLE)
        ui->boxRole->setCurrentText("Throttle");

    else if (role == kfly_comm::enums::RCInput_Role::ROLE_PITCH)
        ui->boxRole->setCurrentText("Pitch");

    else if (role == kfly_comm::enums::RCInput_Role::ROLE_ROLL)
        ui->boxRole->setCurrentText("Roll");

    else if (role == kfly_comm::enums::RCInput_Role::ROLE_YAW)
        ui->boxRole->setCurrentText("Yaw");

    else if (role == kfly_comm::enums::RCInput_Role::ROLE_AUX1)
        ui->boxRole->setCurrentText("Aux 1");

    else if (role == kfly_comm::enums::RCInput_Role::ROLE_AUX2)
        ui->boxRole->setCurrentText("Aux 2");

    else if (role == kfly_comm::enums::RCInput_Role::ROLE_AUX3)
        ui->boxRole->setCurrentText("Aux 3");

    else if (role == kfly_comm::enums::RCInput_Role::ROLE_FLIGHTMODE)
        ui->boxRole->setCurrentText("Flight Mode");

    else if (role == kfly_comm::enums::RCInput_Role::ROLE_ARM_NONLATCH)
        ui->boxRole->setCurrentText("Arming Switch");

    else if (role == kfly_comm::enums::RCInput_Role::ROLE_ENABLE_SERIAL_CONTROL)
        ui->boxRole->setCurrentText("Serial Control");

    else
        ui->boxRole->setCurrentText("Disabled");

}

void ui_rc_input_channel::set_type(kfly_comm::enums::RCInput_Type type)
{
    if (type == kfly_comm::enums::RCInput_Type::TYPE_ON_OFF)
        ui->boxType->setCurrentText("2-state Switch");

    else if (type == kfly_comm::enums::RCInput_Type::TYPE_3_STATE)
        ui->boxType->setCurrentText("3-state Switch");

    else
        ui->boxType->setCurrentText("Analog");
}

void ui_rc_input_channel::set_inverted(bool inverted)
{
    ui->buttonInvert->setChecked(inverted);
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
    else if (text == "Throttle")
    {
        ui->boxType->setCurrentIndex(0);
        ui->boxType->setEnabled(false);
        ui->spinBottom->setEnabled(true);
        ui->spinCenter->setEnabled(false);
        ui->spinCenter->setValue(ui->spinBottom->value());
        ui->spinTop->setEnabled(true);
        ui->buttonInvert->setEnabled(true);
    }
    else if (text == "Pitch" ||
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
    if (ui->boxRole->currentText() == "Throttle")
    {
        if (value > ui->spinTop->value())
            ui->spinBottom->setValue(ui->spinTop->value());

        ui->spinCenter->setValue(ui->spinBottom->value());
    }
    else if (value > ui->spinCenter->value())
        ui->spinBottom->setValue(ui->spinCenter->value());

    emit value_changed();
}

void ui_rc_input_channel::on_spinCenter_valueChanged(int value)
{
    if (ui->boxRole->currentText() != "Throttle")
    {
        if (value < ui->spinBottom->value())
            ui->spinCenter->setValue(ui->spinBottom->value());
        else if (value > ui->spinTop->value())
            ui->spinCenter->setValue(ui->spinTop->value());
    }

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
