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
    else if (mode == RCOutput_Mode::RCOUTPUT_MODE_OPM)
        ui->boxOutputMode->setCurrentText("OneShot 125");
}

kfly_comm::enums::RCOutput_Mode ui_rc_output_channel::get_mode()
{
    using namespace kfly_comm::enums;

    if (ui->boxOutputMode->currentText() == "50 Hz PWM")
        return RCOutput_Mode::RCOUTPUT_MODE_50HZ_PWM;
    else if (ui->boxOutputMode->currentText() == "400 Hz PWM")
        return RCOutput_Mode::RCOUTPUT_MODE_400HZ_PWM;
    else if (ui->boxOutputMode->currentText() == "OneShot 125")
        return RCOutput_Mode::RCOUTPUT_MODE_OPM;
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
