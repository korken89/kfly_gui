#include "communication.h"


communication::communication(QObject *parent)
    : QObject(parent)
{
  // KFly datagram registrations
  _kfly_comm.register_callback(this, &communication::regPing);
  _kfly_comm.register_callback(this, &communication::regSystemStrings);
  _kfly_comm.register_callback(this, &communication::regSystemStatus);
  _kfly_comm.register_callback(this, &communication::regRCInputSettings);
  _kfly_comm.register_callback(this, &communication::regRCValues);
  _kfly_comm.register_callback(this, &communication::regRCOutputSettings);
  _kfly_comm.register_callback(this, &communication::regControlSignals);
  _kfly_comm.register_callback(this, &communication::regChannelMix);
  _kfly_comm.register_callback(this, &communication::regArmSettings);
  _kfly_comm.register_callback(this, &communication::regRateControllerData);
  _kfly_comm.register_callback(this, &communication::regAttitudeControllerData);
  _kfly_comm.register_callback(this, &communication::regControllerLimits);
  _kfly_comm.register_callback(this, &communication::regIMUCalibration);
  _kfly_comm.register_callback(this, &communication::regRawIMUData);
  _kfly_comm.register_callback(this, &communication::regIMUData);
  _kfly_comm.register_callback(this, &communication::regControlFilterSettings);

  // Reserve some space for the transmit buffer
  _transmitt_buffer.reserve(10000);

  connect(&_serialport, &QSerialPort::readyRead,
          this, &communication::parseSerialData);

  connect(&_serialport,
          static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(
                  &QSerialPort::error),
          this, &communication::handleSerialError);

    connect(&_transmit_timer, &QTimer::timeout,
            this, &communication::transmit_buffer);

    _transmit_timer.start(10);
}

communication::~communication()
{
    closePort();
}

bool communication::openPort(const QString& portname, int baudrate)
{
    std::lock_guard<std::mutex> lock(_serialmutex);

    closePort();

    if (portname.trimmed() == "" || baudrate < 9600)
    {
        qDebug() << "Failed to open port " << portname << ", with speed " <<
                    baudrate << " baud. Invalid arguments.";
        return false;
    }
    else
    {
        _serialport.setPortName(portname);
        _serialport.setBaudRate(baudrate);
        _serialport.setParity(QSerialPort::Parity::NoParity);
        _serialport.setStopBits(QSerialPort::StopBits::OneStop);

        return _serialport.open(QIODevice::ReadWrite);
    }
}

void communication::closePort()
{

    if (_serialport.isOpen())
    {
        _serialport.close();
    }
    _transmitt_buffer.clear();
}

void communication::send(const std::vector<uint8_t>& buf)
{
    std::lock_guard<std::mutex> lock(_serialmutex);
    _transmitt_buffer.insert(_transmitt_buffer.end(), buf.cbegin(), buf.cend());
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
    for (auto b : _serialport.readAll())
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

        std::lock_guard<std::mutex> lock(_serialmutex);
        closePort();
    }
}

void communication::transmit_buffer()
{
    std::lock_guard<std::mutex> lock(_serialmutex);

    if (_serialport.isOpen() && !_transmitt_buffer.empty())
    {
        auto size = _transmitt_buffer.size();
        QByteArray data = QByteArray(reinterpret_cast<const char*>(_transmitt_buffer.data()),
                                     size);
        _transmitt_buffer.clear();

        auto wr = _serialport.write(data);

        if (wr != static_cast<decltype(wr)>(size))
            qDebug() << "Error occured when writing data to serial port, size: "
                     << size << ", code: " << wr;
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

void communication::regRCInputSettings(kfly_comm::datagrams::RCInputSettings msg)
{
    emit sigRCInputSettings(msg);
}

void communication::regRCValues(kfly_comm::datagrams::RCValues msg)
{
    emit sigRCValues(msg);
}

void communication::regRCOutputSettings(kfly_comm::datagrams::RCOutputSettings msg)
{
    emit sigRCOutputSettings(msg);
}

void communication::regControlSignals(kfly_comm::datagrams::ControlSignals msg)
{
    emit sigControlSignals(msg);
}

void communication::regChannelMix(kfly_comm::datagrams::ChannelMix msg)
{
    emit sigChannelMix(msg);
}

void communication::regArmSettings(kfly_comm::datagrams::ArmSettings msg)
{
    emit sigArmSettings(msg);
}

void communication::regRateControllerData(kfly_comm::datagrams::RateControllerData msg)
{
    emit sigRateControllerData(msg);
}

void communication::regAttitudeControllerData(kfly_comm::datagrams::AttitudeControllerData msg)
{
    emit sigAttitudeControllerData(msg);
}

void communication::regControllerLimits(kfly_comm::datagrams::ControllerLimits msg)
{
    emit sigControllerLimits(msg);
}

void communication::regIMUCalibration(kfly_comm::datagrams::IMUCalibration msg)
{
    emit sigIMUCalibration(msg);
}

void communication::regRawIMUData(kfly_comm::datagrams::RawIMUData msg)
{
    emit sigRawIMUData(msg);
}

void communication::regIMUData(kfly_comm::datagrams::IMUData msg)
{
    emit sigIMUData(msg);
}

void communication::regControlFilterSettings(kfly_comm::datagrams::ControlFilterSettings msg)
{
    emit sigControlFilterSettings(msg);
}
