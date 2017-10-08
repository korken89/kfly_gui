#include "ui_rc_output_channel.h"
#include "ui_ui_rc_output_channel.h"

ui_rc_output_channel::ui_rc_output_channel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_rc_output_channel)
{
    ui->setupUi(this);

    ui->boxOutputMode->addItem("50 Hz PWM");
    ui->boxOutputMode->addItem("400 Hz PWM");
    ui->boxOutputMode->addItem("OneShot 125");
    ui->boxOutputMode->addItem("OneShot 42");
    ui->boxOutputMode->addItem("MultiShot");
    ui->boxOutputMode->addItem("DShot 150");
    ui->boxOutputMode->addItem("DShot 300");
    ui->boxOutputMode->addItem("DShot 600");
    ui->boxOutputMode->addItem("DShot 1200");
}

ui_rc_output_channel::~ui_rc_output_channel()
{
    delete ui;
}

void ui_rc_output_channel::set_channel_number(int i)
{
    ui->labelChannel->setText(QString::number(i));
}

void ui_rc_output_channel::set_mode_enabled(bool enabled)
{
    ui->boxOutputMode->setEnabled(enabled);
}

void ui_rc_output_channel::set_mode(kfly_comm::enums::RCOutput_Mode mode)
{
    using namespace kfly_comm::enums;

    if (mode == RCOutput_Mode::RCOUTPUT_MODE_50HZ_PWM)
        ui->boxOutputMode->setCurrentText("50 Hz PWM");
    else if (mode == RCOutput_Mode::RCOUTPUT_MODE_400HZ_PWM)
        ui->boxOutputMode->setCurrentText("400 Hz PWM");
    else if (mode == RCOutput_Mode::RCOUTPUT_MODE_ONESHOT125)
        ui->boxOutputMode->setCurrentText("OneShot 125");
    else if (mode == RCOutput_Mode::RCOUTPUT_MODE_ONESHOT42)
        ui->boxOutputMode->setCurrentText("OneShot 42");
    else if (mode == RCOutput_Mode::RCOUTPUT_MODE_MULTISHOT)
        ui->boxOutputMode->setCurrentText("MultiShot");
    else if (mode == RCOutput_Mode::RCOUTPUT_MODE_DSHOT150)
        ui->boxOutputMode->setCurrentText("DShot 150");
    else if (mode == RCOutput_Mode::RCOUTPUT_MODE_DSHOT300)
        ui->boxOutputMode->setCurrentText("DShot 300");
    else if (mode == RCOutput_Mode::RCOUTPUT_MODE_DSHOT600)
        ui->boxOutputMode->setCurrentText("DShot 600");
    else if (mode == RCOutput_Mode::RCOUTPUT_MODE_DSHOT1200)
        ui->boxOutputMode->setCurrentText("DShot 1200");
}

kfly_comm::enums::RCOutput_Mode ui_rc_output_channel::get_mode()
{
    using namespace kfly_comm::enums;

    if (ui->boxOutputMode->currentText() == "50 Hz PWM")
        return RCOutput_Mode::RCOUTPUT_MODE_50HZ_PWM;
    else if (ui->boxOutputMode->currentText() == "400 Hz PWM")
        return RCOutput_Mode::RCOUTPUT_MODE_400HZ_PWM;
    else if (ui->boxOutputMode->currentText() == "OneShot 125")
        return RCOutput_Mode::RCOUTPUT_MODE_ONESHOT125;
    else if (ui->boxOutputMode->currentText() == "OneShot 42")
        return RCOutput_Mode::RCOUTPUT_MODE_ONESHOT42;
    else if (ui->boxOutputMode->currentText() == "MultiShot")
        return RCOutput_Mode::RCOUTPUT_MODE_MULTISHOT;
    else if (ui->boxOutputMode->currentText() == "DShot 150")
        return RCOutput_Mode::RCOUTPUT_MODE_DSHOT150;
    else if (ui->boxOutputMode->currentText() == "DShot 300")
        return RCOutput_Mode::RCOUTPUT_MODE_DSHOT300;
    else if (ui->boxOutputMode->currentText() == "DShot 600")
        return RCOutput_Mode::RCOUTPUT_MODE_DSHOT600;
    else if (ui->boxOutputMode->currentText() == "DShot 1200")
        return RCOutput_Mode::RCOUTPUT_MODE_DSHOT1200;
    else
        return RCOutput_Mode::RCOUTPUT_MODE_50HZ_PWM;
}

void ui_rc_output_channel::set_channel_enabled(bool enabled)
{
    return ui->buttonEnable->setChecked(enabled);
}

bool ui_rc_output_channel::get_channel_enabled()
{
    return ui->buttonEnable->isChecked();
}

void ui_rc_output_channel::set_bar_value(float value)
{
    ui->barCurrentValue->setValue((int)(value * 100));
}

void ui_rc_output_channel::set_override_enabled(bool enabled)
{
    ui->sliderSetValue->setEnabled(enabled);
}

void ui_rc_output_channel::set_override_value(int val)
{
    if (ui->buttonEnable->isChecked())
        ui->sliderSetValue->setValue(val);
}

int ui_rc_output_channel::get_override_value()
{
    return ui->sliderSetValue->value();
}

void ui_rc_output_channel::on_buttonEnable_toggled(bool)
{
    emit value_changed();
}

void ui_rc_output_channel::on_boxOutputMode_currentTextChanged(const QString &)
{
    emit value_changed();
}

void ui_rc_output_channel::on_sliderSetValue_sliderMoved(int)
{
    if (!ui->buttonEnable->isChecked())
        ui->sliderSetValue->setValue(0);

    emit value_changed();
}
