#include "communication.h"

#include <iomanip>
#include <thread>

communication::communication(QObject *parent)
    : QObject(parent)
{
  _kfly_comm.register_callback(this, &communication::regPing);
  _kfly_comm.register_callback(this, &communication::regSystemInformation);

  connect(&_serialport, &QSerialPort::readyRead, this, &communication::parseSerialData);

}

communication::~communication()
{
    closePort();
}

bool communication::openPort(const QString& portname, int baudrate)
{
    closePort();

    std::lock_guard<std::mutex> lock(_serialmutex);

    if (portname.trimmed() == "" || baudrate < 9600)
    {
        qDebug() << "Failed to open port " << portname << ", with speed " << baudrate << " baud. Invalid arguments.";
        return false;
    }

    _serialport.setPortName(portname);
    _serialport.setBaudRate(baudrate);

    return _serialport.open(QIODevice::ReadWrite);
}

void communication::closePort()
{
    std::lock_guard<std::mutex> lock(_serialmutex);

    if (_serialport.isOpen())
    {
        _serialport.close();
    }
}

void communication::send(const std::vector<uint8_t>& buf)
{
    std::lock_guard<std::mutex> lock(_serialmutex);

    if (_serialport.isOpen())
    {
        QByteArray data = QByteArray(reinterpret_cast<const char*>(buf.data()), buf.size());
        auto wr = _serialport.write(data);

        if (wr != static_cast<decltype(wr)>(buf.size()))
            qDebug() << "Error occured when writing data to serial port, size: " << buf.size() << ", code: " << wr;

        _serialport.waitForBytesWritten(100);
    }
}

void communication::parseSerialData()
{
    QByteArray inByteArray;

    {   /* Lock only while reading the port. */
        std::lock_guard<std::mutex> lock(_serialmutex);
        inByteArray = _serialport.readAll();
    }

    qDebug() << "Data received!";

    for (auto b : inByteArray)
    {
        _kfly_comm.parse(b);
        QString valueInHex = QString("0x%1").arg(static_cast<unsigned int>(b) & 0xff, 0, 16);
        qDebug() << valueInHex;
    }
}

void communication::regPing(kfly_comm::datagrams::Ping)
{
    qDebug() << "Ping ping ping!!!";
    emit sigPing();
}

void communication::regSystemInformation(kfly_comm::datagrams::SystemInformation msg)
{
    qDebug() << "SysInfo SysInfo SysInfo!!!";
    emit sigSystemInformation(msg);
}
