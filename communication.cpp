#include "communication.h"

#include <thread>

communication::communication(QObject *parent)
    : QObject(parent)
{
  /* KFly datagram registrations */
  _kfly_comm.register_callback(this, &communication::regPing);
  _kfly_comm.register_callback(this, &communication::regSystemStrings);
  _kfly_comm.register_callback(this, &communication::regSystemStatus);

  connect(&_serialport, &QSerialPort::readyRead,
          this, &communication::parseSerialData);

  connect(&_serialport, &QSerialPort::errorOccurred,
          this, &communication::handleSerialError);

}

communication::~communication()
{
    closePort();
}

bool communication::openPort(const QString& portname, int baudrate)
{
    closePort();

    if (portname.trimmed() == "" || baudrate < 9600)
    {
        qDebug() << "Failed to open port " << portname << ", with speed " <<
                    baudrate << " baud. Invalid arguments.";
        return false;
    }
    else
    {
        std::lock_guard<std::mutex> lock(_serialmutex);

        _serialport.setPortName(portname);
        _serialport.setBaudRate(baudrate);

        return _serialport.open(QIODevice::ReadWrite);
    }
}

void communication::closePort()
{
    unsubscribe_all();

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
        QByteArray data = QByteArray(reinterpret_cast<const char*>(buf.data()),
                                     buf.size());
        auto wr = _serialport.write(data);

        if (wr != static_cast<decltype(wr)>(buf.size()))
            qDebug() << "Error occured when writing data to serial port, size: "
                     << buf.size() << ", code: " << wr;

        _serialport.waitForBytesWritten(100);
    }
}

void communication::subscribe(kfly_comm::commands cmd, unsigned int dt_ms)
{
    if (dt_ms < 5)
        return;
    else
    {
        kfly_comm::datagrams::ManageSubscription sub;
        sub.cmd = cmd;
        sub.delta_ms = dt_ms;
        sub.subscribe = true;
        sub.port = kfly_comm::enums::Ports::PORT_SAME;

        send(kfly_comm::codec::generate_packet(sub));
    }
}

void communication::unsubscribe(kfly_comm::commands cmd)
{
    kfly_comm::datagrams::ManageSubscription sub;
    sub.cmd = cmd;
    sub.delta_ms = 0;
    sub.subscribe = false;
    sub.port = kfly_comm::enums::Ports::PORT_SAME;

    send(kfly_comm::codec::generate_packet(sub));
}

void communication::unsubscribe_all()
{
    kfly_comm::datagrams::ManageSubscription sub;
    sub.cmd = kfly_comm::commands::None;
    sub.delta_ms = 0xffffffff; // Flag to unsubscribe all
    sub.subscribe = false;
    sub.port = kfly_comm::enums::Ports::PORT_SAME;

    send(kfly_comm::codec::generate_packet(sub));
}

void communication::parseSerialData()
{
    QByteArray inByteArray;

    {   /* Lock only while reading the port. */
        std::lock_guard<std::mutex> lock(_serialmutex);
        inByteArray = _serialport.readAll();
    }

    for (auto b : inByteArray)
    {
        _kfly_comm.parse(b);

        //qDebug() << QString("0x%1").arg(static_cast<unsigned int>(b) & 0xff,
        //                                0, 16);
    }
}

void communication::handleSerialError(QSerialPort::SerialPortError error)
{
    qDebug() << "Serial error: " << _serialport.errorString();

    if (error != QSerialPort::NoError)
    {
        emit sigConnectionError();
        closePort();
    }
}

void communication::regPing(kfly_comm::datagrams::Ping)
{
    emit sigPing();
}

void communication::regSystemStrings(
        kfly_comm::datagrams::SystemStrings msg)
{
    emit sigSystemStrings(msg);
}

void communication::regSystemStatus(
        kfly_comm::datagrams::SystemStatus msg)
{
    emit sigSystemStatus(msg);
}
