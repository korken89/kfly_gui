#include "ui_connect.h"
#include "ui_ui_connect.h"

#include <QDebug>

ui_connect::ui_connect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_connect)
{
    ui->setupUi(this);


    ui->boxBaud->addItem("9600");
    ui->boxBaud->addItem("19200");
    ui->boxBaud->addItem("38400");
    ui->boxBaud->addItem("57600");
    ui->boxBaud->addItem("115200");
    ui->boxBaud->addItem("1000000");
    ui->boxBaud->setCurrentIndex(ui->boxBaud->count() - 1);

    for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
    {
        qDebug() << port.portName() << port.vendorIdentifier() << port.productIdentifier()
                 << port.description() << port.manufacturer() << port.serialNumber()
                 << port.systemLocation()
                 << port.hasProductIdentifier() << port.hasVendorIdentifier() << port.isBusy();

        ui->boxPort->addItem(port.portName());
    }
}

ui_connect::~ui_connect()
{
    delete ui;
}


void ui_connect::register_communication(communication *com)
{
    _communication = com;
}

void ui_connect::on_buttonConnect_clicked()
{
    if (_communication == nullptr)
        return;

    if (ui->buttonConnect->text() == "Connect")
    {
        auto port = ui->boxPort->itemText(ui->boxPort->currentIndex());
        auto baud = ui->boxBaud->itemText(ui->boxBaud->currentIndex());

        if (_communication->openPort(port, baud.toInt()))
        {
            ui->buttonConnect->setText("Disconnect");
            qDebug() << "Opening port " << port << ", with speed " << baud.trimmed() << " baud";

            auto data = kfly_comm::codec::generate_command(kfly_comm::commands::Ping);
            _communication->send(data);
        }
        else
            qDebug() << "Error connecting to port " << port << ", with speed " << baud.trimmed() << " baud";
    }
    else
    {
        _communication->closePort();
        ui->buttonConnect->setText("Connect");
    }
}
