#include "ui_connect.h"
#include "ui_ui_connect.h"
#include "mainwindow.h"

#include <QDebug>

ui_connect::ui_connect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_connect)
{
    ui->setupUi(this);

    connect(&ping_timer, &QTimer::timeout,
            this, &ui_connect::ping_timer_event);

    connect(&check_port_timer, &QTimer::timeout,
            this, &ui_connect::check_port_timer_event);

    ui->boxBaud->addItem("38400");
    ui->boxBaud->addItem("57600");
    ui->boxBaud->addItem("115200");
    ui->boxBaud->addItem("230400");
    ui->boxBaud->addItem("460800");
    ui->boxBaud->addItem("921600");
    ui->boxBaud->addItem("1000000");
    ui->boxBaud->addItem("1500000");
    ui->boxBaud->addItem("2000000");
    ui->boxBaud->addItem("2500000");
    ui->boxBaud->addItem("3000000");

    ui->boxBaud->setCurrentIndex(6);


    check_port_timer.start(1000);
}

ui_connect::~ui_connect()
{
    delete ui;
}


void ui_connect::register_communication(communication *com)
{
    _communication = com;

    connect(_communication, &communication::sigPing,
            this, &ui_connect::ping_received);

    connect(_communication, &communication::sigConnectionError,
            this, &ui_connect::connection_error);
}


void ui_connect::ping_timer_event()
{
    if (ping_counter > 1)
    {
        qDebug() << "Ping timeout!";

        disconnect_port();
    }
    else
    {
        using namespace kfly_comm;

        qDebug() << "Sending ping...";

        ping_counter++;
        auto data = codec::generate_command(commands::Ping);
        _communication->send(data);
    }
}

void ui_connect::check_port_timer_event()
{
    for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
    {
        if (ui->boxPort->findText(port.portName()) < 0)
        {
            qDebug() << port.portName() << port.vendorIdentifier() <<
                        port.productIdentifier() << port.description() <<
                        port.manufacturer() << port.serialNumber() <<
                        port.systemLocation() << port.hasProductIdentifier() <<
                        port.hasVendorIdentifier() << port.isBusy();

            ui->boxPort->addItem(port.portName());
        }
    }
}

void ui_connect::ping_received()
{
    ping_counter = 0;

    if (first_ping)
    {
        first_ping = false;
        emit connection_established();

        qDebug() << "Connection established...";
    }

    emit heartbeat();

    qDebug() << "UI connect: ping";
}

void ui_connect::connection_error()
{
    disconnect_port();

    qDebug() << "UI connect: conn error";
}

void ui_connect::connect_port()
{
    auto port = ui->boxPort->itemText(ui->boxPort->currentIndex());
    auto baud = ui->boxBaud->itemText(ui->boxBaud->currentIndex());

    if (_communication->openPort(port, baud.toInt()))
    {
        ui->buttonConnect->setText("Disconnect");
        qDebug() << "Opening port " << port <<
                    ", with speed " << baud.trimmed() << " baud";

        ping_timer.start(2000);
        ping_counter = 1;
        first_ping = true;

        using namespace kfly_comm;
        _communication->send(codec::generate_command(commands::Ping));
    }
    else
        qDebug() << "Error connecting to port " << port <<
                    ", with speed " << baud.trimmed() << " baud";
}

void ui_connect::disconnect_port()
{
    first_ping = false;

    ping_timer.stop();
    emit connection_lost();

    _communication->unsubscribe_all();
    _communication->closePort();
    ui->buttonConnect->setText("Connect");
}

void ui_connect::on_buttonConnect_clicked()
{
    if (_communication == nullptr)
        return;

    if (ui->buttonConnect->text() == "Connect")
      connect_port();
    else
      disconnect_port();
}
