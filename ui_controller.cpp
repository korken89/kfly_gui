#include "ui_controller.h"
#include "ui_ui_controller.h"

ui_controller::ui_controller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_controller)
{
    ui->setupUi(this);
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

    // TODO: implement this


    //if (_communication != nullptr)
    //    _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_controller::upload_attitude_controller_settings()
{
    qDebug() << "attitude controller upload settings";

    kfly_comm::datagrams::AttitudeControllerData msg;

    // TODO: implement this


    //if (_communication != nullptr)
    //    _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_controller::upload_limits_settings()
{
    qDebug() << "controller limits upload settings";

    kfly_comm::datagrams::ControllerLimits msg;

    // TODO: implement this


    //if (_communication != nullptr)
    //    _communication->send(kfly_comm::codec::generate_packet(msg));
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

    // TODO: implement this

}

void ui_controller::attitude_controller_settings(kfly_comm::datagrams::AttitudeControllerData msg)
{
    qDebug() << "got attitude controller settings";

    // TODO: implement this

}

void ui_controller::limits_settings(kfly_comm::datagrams::ControllerLimits msg)
{
    qDebug() << "got controller limits settings";

    // TODO: implement this

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
