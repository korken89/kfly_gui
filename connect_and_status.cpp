#include "connect_and_status.h"
#include "ui_connect_and_status.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

connect_and_status::connect_and_status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connect_and_status)
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

connect_and_status::~connect_and_status()
{
    delete ui;
}
