#include "ui_arming.h"
#include "ui_ui_arming.h"

ui_arming::ui_arming(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_arming)
{
    ui->setupUi(this);

    _upload_settings = false;
    _auto_upload_checked = false;

    ui->boxArmAction->addItem("Momentary switch");
    ui->boxArmAction->addItem("Pitch min");
    ui->boxArmAction->addItem("Pitch max");
    ui->boxArmAction->addItem("Roll min");
    ui->boxArmAction->addItem("Roll max");
    ui->boxArmAction->addItem("Yaw min");
    ui->boxArmAction->addItem("Yaw max");

    ui->spinAutomaticDisarmTime->setEnabled(false);
    ui->spinSpinWhenArmedThrottle->setEnabled(false);
}

ui_arming::~ui_arming()
{
    delete ui;
}

void ui_arming::register_communication(communication *com)
{
    _communication = com;

    connect(_communication, &communication::sigArmSettings,
            this, &ui_arming::arming_settings);

    connect(&_upload_settings_timer, &QTimer::timeout,
            this, &ui_arming::upload_settings_timer);

    _upload_settings_timer.start(1000);
}

void ui_arming::showEvent(QShowEvent *)
{
}

void ui_arming::hideEvent(QHideEvent *)
{
}

void ui_arming::upload_settings()
{
    qDebug() << "arming upload settings";

    kfly_comm::datagrams::ArmSettings msg;

    if (ui->checkSpinWhenArmed->checkState() == Qt::Checked)
        msg.armed_min_throttle = 0;
    else
        msg.armed_min_throttle = static_cast<float>(ui->spinSpinWhenArmedThrottle->value()) / 100.0f;

    msg.arm_stick_time = static_cast<uint8_t>(ui->spinArmTime->value() * 10 + 0.5);

    if (ui->checkAutomaticDisarm->checkState() == Qt::Checked)
        msg.arm_zero_throttle_timeout = ui->spinAutomaticDisarmTime->value();
    else
        msg.arm_zero_throttle_timeout = 0;

    msg.stick_direction = arming_direction_to_enum();

    msg.stick_threshold = static_cast<float>(ui->spinArmThreshold->value()) / 100.0f;

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

kfly_comm::enums::Arming_Stick_Direction ui_arming::arming_direction_to_enum()
{
    if (ui->boxArmAction->currentText() == "Momentary switch")
        return kfly_comm::enums::Arming_Stick_Direction::NON_LATCHING_SWITCH;
    else if (ui->boxArmAction->currentText() == "Pitch min")
        return kfly_comm::enums::Arming_Stick_Direction::STICK_PITCH_MIN;
    else if (ui->boxArmAction->currentText() == "Pitch max")
        return kfly_comm::enums::Arming_Stick_Direction::STICK_PITCH_MAX;
    else if (ui->boxArmAction->currentText() == "Roll min")
        return kfly_comm::enums::Arming_Stick_Direction::STICK_ROLL_MIN;
    else if (ui->boxArmAction->currentText() == "Roll max")
        return kfly_comm::enums::Arming_Stick_Direction::STICK_ROLL_MAX;
    else if (ui->boxArmAction->currentText() == "Yaw min")
        return kfly_comm::enums::Arming_Stick_Direction::STICK_YAW_MIN;
    else if (ui->boxArmAction->currentText() == "Yaw max")
        return kfly_comm::enums::Arming_Stick_Direction::STICK_YAW_MAX;
    else
        return kfly_comm::enums::Arming_Stick_Direction::NON_LATCHING_SWITCH;
}

void ui_arming::set_arming_direction(kfly_comm::enums::Arming_Stick_Direction dir)
{
    if (dir == kfly_comm::enums::Arming_Stick_Direction::NON_LATCHING_SWITCH)
        ui->boxArmAction->setCurrentText("Momentary switch");
    else if (dir == kfly_comm::enums::Arming_Stick_Direction::STICK_PITCH_MIN)
        ui->boxArmAction->setCurrentText("Pitch min");
    else if (dir == kfly_comm::enums::Arming_Stick_Direction::STICK_PITCH_MAX)
        ui->boxArmAction->setCurrentText("Pitch max");
    else if (dir == kfly_comm::enums::Arming_Stick_Direction::STICK_ROLL_MIN)
        ui->boxArmAction->setCurrentText("Roll min");
    else if (dir == kfly_comm::enums::Arming_Stick_Direction::STICK_ROLL_MAX)
        ui->boxArmAction->setCurrentText("Roll max");
    else if (dir == kfly_comm::enums::Arming_Stick_Direction::STICK_YAW_MIN)
        ui->boxArmAction->setCurrentText("Yaw min");
    else if (dir == kfly_comm::enums::Arming_Stick_Direction::STICK_YAW_MAX)
        ui->boxArmAction->setCurrentText("Yaw max");
    else
        ui->boxArmAction->setCurrentText("Momentary switch");
}

void ui_arming::connection_established()
{
    /* Request on connection. */
    if (_communication != nullptr)
    {
        using namespace kfly_comm;
        _communication->send(codec::generate_command(commands::GetArmSettings));
    }
}

void ui_arming::auto_upload_changed(bool checked)
{
    _auto_upload_checked = checked;
}

void ui_arming::upload_now()
{
    upload_settings();
}

void ui_arming::arming_settings(kfly_comm::datagrams::ArmSettings msg)
{
    qDebug() << "got arming settings";

    if (msg.armed_min_throttle == 0)
    {
        ui->checkSpinWhenArmed->setChecked(false);
        ui->spinSpinWhenArmedThrottle->setValue(1);
    }
    else
    {
        ui->checkSpinWhenArmed->setChecked(true);
        ui->spinSpinWhenArmedThrottle->setValue(msg.armed_min_throttle * 100);
    }

    ui->spinArmTime->setValue(static_cast<double>(msg.arm_stick_time) / 10.0);

    if (msg.arm_zero_throttle_timeout == 0)
    {
        ui->checkAutomaticDisarm->setChecked(false);
        ui->spinAutomaticDisarmTime->setValue(1);
    }
    else
    {
        ui->checkAutomaticDisarm->setChecked(true);
        ui->spinAutomaticDisarmTime->setValue(msg.arm_zero_throttle_timeout);
    }

    set_arming_direction(msg.stick_direction);

    ui->spinArmThreshold->setValue(100 * msg.stick_threshold);
}

void ui_arming::upload_settings_timer()
{
    if (_upload_settings && _auto_upload_checked)
    {
        _upload_settings = false;
        upload_settings();
    }
}

void ui_arming::on_checkAutomaticDisarm_toggled(bool checked)
{
    ui->spinAutomaticDisarmTime->setEnabled(checked);

    if (!isHidden())
        _upload_settings = true;
}

void ui_arming::on_checkSpinWhenArmed_toggled(bool checked)
{
    ui->spinSpinWhenArmedThrottle->setEnabled(checked);

    if (!isHidden())
        _upload_settings = true;
}

void ui_arming::on_boxArmAction_currentIndexChanged(int)
{
    if (!isHidden())
        _upload_settings = true;
}

void ui_arming::on_spinArmThreshold_valueChanged(int)
{
    if (!isHidden())
        _upload_settings = true;
}

void ui_arming::on_spinArmTime_valueChanged(double)
{
    if (!isHidden())
        _upload_settings = true;
}

void ui_arming::on_spinAutomaticDisarmTime_valueChanged(int)
{
    if (!isHidden())
        _upload_settings = true;
}

void ui_arming::on_spinSpinWhenArmedThrottle_valueChanged(int)
{
    if (!isHidden())
        _upload_settings = true;
}
