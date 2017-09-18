#include "ui_controller.h"
#include "ui_ui_controller.h"

ui_controller::ui_controller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_controller)
{
    ui->setupUi(this);

    _upload_rate_controller_settings = false;
    _upload_attitude_controller_settings = false;
    _upload_limit_settings = false;
}

ui_controller::~ui_controller()
{
    delete ui;
}

void ui_controller::register_communication(communication *com)
{
    _communication = com;

    connect(_communication, &communication::sigRateControllerData,
            this, &ui_controller::rate_controller_settings);

    connect(_communication, &communication::sigAttitudeControllerData,
            this, &ui_controller::attitude_controller_settings);

    connect(_communication, &communication::sigControllerLimits,
            this, &ui_controller::limits_settings);

    connect(&_upload_settings_timer, &QTimer::timeout,
            this, &ui_controller::upload_settings_timer);

    _upload_settings_timer.start(1000);
}

void ui_controller::showEvent(QShowEvent *)
{
}

void ui_controller::hideEvent(QHideEvent *)
{
}

void ui_controller::upload_rate_controller_settings()
{
    qDebug() << "rate controller upload settings";

    kfly_comm::datagrams::RateControllerData msg;

    msg.roll_controller.P_gain = ui->spinRateRoll_p->value();
    msg.roll_controller.I_gain = ui->spinRateRoll_i->value();

    msg.pitch_controller.P_gain = ui->spinRatePitch_p->value();
    msg.pitch_controller.I_gain = ui->spinRatePitch_i->value();

    msg.yaw_controller.P_gain = ui->spinRateYaw_p->value();
    msg.yaw_controller.I_gain = ui->spinRateYaw_i->value();

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_controller::upload_attitude_controller_settings()
{
    qDebug() << "attitude controller upload settings";

    kfly_comm::datagrams::AttitudeControllerData msg;

    msg.roll_controller.P_gain = ui->spinAttitudeRoll_p->value();
    msg.roll_controller.I_gain = ui->spinAttitudeRoll_i->value();

    msg.pitch_controller.P_gain = ui->spinAttitudePitch_p->value();
    msg.pitch_controller.I_gain = ui->spinAttitudePitch_i->value();

    // TODO: For future use
    msg.yaw_controller.P_gain = 0;
    msg.yaw_controller.I_gain = 0;

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_controller::upload_limits_settings()
{
    qDebug() << "controller limits upload settings";

    kfly_comm::datagrams::ControllerLimits msg;

    msg.max_rate.roll = to_radians(ui->spinLimitRateRoll->value());
    msg.max_rate.pitch = to_radians(ui->spinLimitRatePitch->value());
    msg.max_rate.yaw = to_radians(ui->spinLimitRateYaw->value());

    msg.max_angle.roll = to_radians(ui->spinLimitAttitudeRoll->value());
    msg.max_angle.pitch = to_radians(ui->spinLimitAttitudePitch->value());

    msg.max_rate_attitude.roll = to_radians(ui->spinLimitAttitudeRateRoll->value());
    msg.max_rate_attitude.pitch = to_radians(ui->spinLimitAttitudeRatePitch->value());
    msg.max_rate_attitude.yaw = to_radians(ui->spinLimitAttitudeRateYaw->value());

    msg.max_velocity.horizontal = 0;
    msg.max_velocity.vertical = 0;

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

double ui_controller::to_radians(double in)
{
    return (in / 180.0) * M_PI;
}

double ui_controller::to_degrees(double in)
{

    return (in * 180.0) / M_PI;
}


void ui_controller::connection_established()
{
    /* Request on connection. */
    if (_communication != nullptr)
    {
        using namespace kfly_comm;
        _communication->send(codec::generate_command(commands::GetRateControllerData));
        _communication->send(codec::generate_command(commands::GetAttitudeControllerData));
        _communication->send(codec::generate_command(commands::GetControllerLimits));
    }
}

void ui_controller::auto_upload_changed(bool checked)
{
    _auto_upload_checked = checked;
}

void ui_controller::upload_now()
{
    upload_rate_controller_settings();
    upload_attitude_controller_settings();
    upload_limits_settings();
}

void ui_controller::rate_controller_settings(kfly_comm::datagrams::RateControllerData msg)
{
    qDebug() << "got rate controller settings";

    ui->spinRateRoll_p->setValue(msg.roll_controller.P_gain);
    ui->spinRateRoll_i->setValue(msg.roll_controller.I_gain);

    ui->spinRatePitch_p->setValue(msg.pitch_controller.P_gain);
    ui->spinRatePitch_i->setValue(msg.pitch_controller.I_gain);

    ui->spinRateYaw_p->setValue(msg.yaw_controller.P_gain);
    ui->spinRateYaw_i->setValue(msg.yaw_controller.I_gain);

}

void ui_controller::attitude_controller_settings(kfly_comm::datagrams::AttitudeControllerData msg)
{
    qDebug() << "got attitude controller settings";

    ui->spinAttitudeRoll_p->setValue(msg.roll_controller.P_gain);
    ui->spinAttitudeRoll_i->setValue(msg.roll_controller.I_gain);

    ui->spinAttitudePitch_p->setValue(msg.pitch_controller.P_gain);
    ui->spinAttitudePitch_i->setValue(msg.pitch_controller.I_gain);
}

void ui_controller::limits_settings(kfly_comm::datagrams::ControllerLimits msg)
{
    qDebug() << "got controller limits settings";

    ui->spinLimitRateRoll->setValue(to_degrees(msg.max_rate.roll) + 0.5);
    ui->spinLimitRatePitch->setValue(to_degrees(msg.max_rate.pitch) + 0.5);
    ui->spinLimitRateYaw->setValue(to_degrees(msg.max_rate.yaw) + 0.5);

    ui->spinLimitAttitudeRoll->setValue(to_degrees(msg.max_angle.roll) + 0.5);
    ui->spinLimitAttitudePitch->setValue(to_degrees(msg.max_angle.pitch) + 0.5);

    ui->spinLimitAttitudeRateRoll->setValue(to_degrees(msg.max_rate_attitude.roll) + 0.5);
    ui->spinLimitAttitudeRatePitch->setValue(to_degrees(msg.max_rate_attitude.pitch) + 0.5);
    ui->spinLimitAttitudeRateYaw->setValue(to_degrees(msg.max_rate_attitude.yaw) + 0.5);

}

void ui_controller::upload_settings_timer()
{
    if (_upload_rate_controller_settings && _auto_upload_checked)
    {
        _upload_rate_controller_settings = false;
        upload_rate_controller_settings();
    }

    if (_upload_attitude_controller_settings && _auto_upload_checked)
    {
        _upload_attitude_controller_settings = false;
        upload_attitude_controller_settings();
    }

    if (_upload_limit_settings && _auto_upload_checked)
    {
        _upload_limit_settings = false;
        upload_limits_settings();
    }
}

//
// Changes to rate controller gains
//
void ui_controller::on_spinRateRoll_p_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}

void ui_controller::on_spinRateRoll_i_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}

void ui_controller::on_spinRatePitch_p_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}

void ui_controller::on_spinRatePitch_i_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}

void ui_controller::on_spinRateYaw_p_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}

void ui_controller::on_spinRateYaw_i_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}



//
// Changes to attitude controller gains
//

void ui_controller::on_spinAttitudeRoll_p_valueChanged(double)
{
    if (!isHidden())
        _upload_attitude_controller_settings = true;
}

void ui_controller::on_spinAttitudeRoll_i_valueChanged(double)
{
    if (!isHidden())
        _upload_attitude_controller_settings = true;
}

void ui_controller::on_spinAttitudePitch_p_valueChanged(double)
{
    if (!isHidden())
        _upload_attitude_controller_settings = true;
}

void ui_controller::on_spinAttitudePitch_i_valueChanged(double)
{
    if (!isHidden())
        _upload_attitude_controller_settings = true;
}



//
// Changes to limits
//
void ui_controller::on_spinLimitRateRoll_valueChanged(int)
{
    if (!isHidden())
        _upload_limit_settings = true;
}

void ui_controller::on_spinLimitRatePitch_valueChanged(int)
{
    if (!isHidden())
        _upload_limit_settings = true;
}

void ui_controller::on_spinLimitRateYaw_valueChanged(int)
{
    if (!isHidden())
        _upload_limit_settings = true;
}

void ui_controller::on_spinLimitAttitudeRateRoll_valueChanged(int)
{
    if (!isHidden())
        _upload_limit_settings = true;
}

void ui_controller::on_spinLimitAttitudeRoll_valueChanged(int)
{
    if (!isHidden())
        _upload_limit_settings = true;
}

void ui_controller::on_spinLimitAttitudeRatePitch_valueChanged(int)
{
    if (!isHidden())
        _upload_limit_settings = true;
}

void ui_controller::on_spinLimitAttitudePitch_valueChanged(int)
{
    if (!isHidden())
        _upload_limit_settings = true;
}

void ui_controller::on_spinLimitAttitudeRateYaw_valueChanged(int)
{
    if (!isHidden())
        _upload_limit_settings = true;
}
