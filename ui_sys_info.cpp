#include "ui_sys_info.h"
#include "ui_ui_sys_info.h"

ui_sys_info::ui_sys_info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_sys_info)
{
    ui->setupUi(this);

    _auto_upload_checked = false;
    _upload_settings = false;
}

ui_sys_info::~ui_sys_info()
{
    delete ui;
}

void ui_sys_info::register_communication(communication *com)
{
    _communication = com;

    connect(_communication, &communication::sigSystemStrings,
            this, &ui_sys_info::system_strings);

    connect(_communication, &communication::sigSystemStatus,
            this, &ui_sys_info::system_status);

    connect(&_upload_settings_timer, &QTimer::timeout,
            this, &ui_sys_info::upload_settings_timer);

    _upload_settings_timer.start(1000);

}

void ui_sys_info::showEvent(QShowEvent *)
{
    if (_communication != nullptr)
        _communication->subscribe(kfly_comm::commands::GetSystemStatus, 200);
}

void ui_sys_info::hideEvent(QHideEvent *)
{
    if (_communication != nullptr)
        _communication->unsubscribe(kfly_comm::commands::GetSystemStatus);
}

void ui_sys_info::upload_settings()
{
    qDebug() << "system strings upload";

    kfly_comm::datagrams::SetDeviceStrings msg;
    msg.SetStrings(ui->editVehicleName->text().toLatin1().data(),
                   ui->editVehicleType->text().toLatin1().data());

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_sys_info::connection_established()
{
    /* Request system information on connection. */
    if (_communication != nullptr)
    {
        using namespace kfly_comm;
        _communication->send(codec::generate_command(commands::GetSystemStrings));

        if (!isHidden())
            _communication->subscribe(kfly_comm::commands::GetSystemStatus, 200);
    }
}

void ui_sys_info::auto_upload_changed(bool checked)
{
    _auto_upload_checked = checked;
}

void ui_sys_info::upload_now()
{
    upload_settings();
}

void ui_sys_info::system_strings(kfly_comm::datagrams::SystemStrings msg)
{
    QString vehicle_name = msg.vehicle_name;
    QString vehicle_type = msg.vehicle_type;
    QString kfly_version = msg.kfly_version;
    QString uid = QByteArray(reinterpret_cast<char *>(msg.unique_id), 12).toBase64();

    ui->editVehicleName->setText(vehicle_name);
    ui->editVehicleType->setText(vehicle_type);
    ui->editVersion->setText(kfly_version);
    ui->editUniqueID->setText(uid);
}

void ui_sys_info::system_status(kfly_comm::datagrams::SystemStatus msg)
{
    QString uptime = QString::number(qRound(msg.up_time*10) / 10.0, 'f', 1);
    ui->editUpTime->setText(uptime);

    QString battery_voltage = QString::number(qRound(msg.up_time*100) / 100.0, 'f', 2);
    if (msg.battery_voltage < 0)
        ui->editBatteryVoltage->setText("N/A");
    else
        ui->editBatteryVoltage->setText(battery_voltage);

    QString flight_time = QString::number(qRound(msg.flight_time*10) / 10.0, 'f', 1);
    if (msg.flight_time < 0)
        ui->editFlightTime->setText("N/A");
    else
        ui->editFlightTime->setText(flight_time);

    if (msg.cpu_usage < 0)
        ui->progressCPU->setValue(0);
    else
        ui->progressCPU->setValue(msg.cpu_usage * 100);


    ui->buttonArmed->setChecked(msg.motors_armed);
    ui->buttonSerialInterface->setChecked(msg.serial_interface_enabled);
    ui->buttonInAir->setChecked(msg.in_air);

}

void ui_sys_info::upload_settings_timer()
{
    if (_upload_settings && _auto_upload_checked)
    {
        _upload_settings = false;
        upload_settings();
    }
}

void ui_sys_info::on_editVehicleType_textChanged(const QString &)
{
    _upload_settings = true;
}

void ui_sys_info::on_editVehicleName_textChanged(const QString &)
{
    _upload_settings = true;
}
