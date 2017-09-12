#include "ui_arming.h"
#include "ui_ui_arming.h"

ui_arming::ui_arming(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_arming)
{
    ui->setupUi(this);

    ui->boxArmAction->addItem("None");
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


    //	TODO: fill out the message


    //	if (_communication != nullptr)
    //	    _communication->send(kfly_comm::codec::generate_packet(msg));
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
    // TODO: implement this
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
}

void ui_arming::on_checkSpinWhenArmed_toggled(bool checked)
{
    ui->spinSpinWhenArmedThrottle->setEnabled(checked);
}
