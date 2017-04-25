#include "ui_sys_info.h"
#include "ui_ui_sys_info.h"

ui_sys_info::ui_sys_info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_sys_info)
{
    ui->setupUi(this);
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

}

void ui_sys_info::connection_established()
{
    /* Request system information on connection. */
    if (_communication != nullptr)
    {
        using namespace kfly_comm;
        _communication->send(codec::generate_command(commands::GetSystemStrings));
        _communication->subscribe(kfly_comm::commands::GetSystemStatus, 200);
    }
}

void ui_sys_info::system_strings(kfly_comm::datagrams::SystemStrings msg)
{
    QString vehicle_name = msg.vehicle_name;
    QString vehicle_type = msg.vehicle_type;
    QString kfly_version = msg.kfly_version;
    QString uid = QByteArray(reinterpret_cast<char *>(msg.unique_id), 12).toBase64();

    if (ui->editUniqueID->text() != uid)
    {
        ui->editVehicleName->setText(vehicle_name);
        ui->editVehicleType->setText(vehicle_type);
        ui->editVersion->setText(kfly_version);
        ui->editUniqueID->setText(uid);
    }
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
