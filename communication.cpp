#include "communication.h"

#include <iomanip>

communication::communication(QObject *parent)
    : QObject(parent)
{
  connect(&_serialport, &QSerialPort::readyRead, this, &communication::parseSerialData);
}

communication::~communication()
{
    closePort();
}

void communication::openPort(QString portname, int baudrate)
{
    closePort();


    _serialport.setPortName(portname);
    _serialport.setBaudRate(baudrate);

    qDebug() << "Opening port " << portname << ", with speed " << baudrate << " baud";

    _serialport.open(QIODevice::ReadWrite);
}

void communication::closePort()
{
    if (_serialport.isOpen())
    {
        _serialport.close();
    }
}

void communication::send(const std::vector<uint8_t>& buf)
{
    if (_serialport.isOpen())
    {
        //QByteArray* img = new QByteArray(reinterpret_cast<const char*>(buf.data()), buf.size());
        _serialport.write(reinterpret_cast<const char*>(buf.data()), buf.size());
    }
}

void communication::parseSerialData()
{
    QByteArray inByteArray = _serialport.readAll();

    for (auto b : inByteArray)
    {
        QString valueInHex = QString("%1").arg(static_cast<int>(b), 0, 16);
        qDebug() << "0x" << valueInHex;
    }
}
