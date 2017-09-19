#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QtCore>
#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <vector>
#include "kfly_comm/kfly_comm.hpp"

class communication : public QObject
{
    Q_OBJECT

private:

    QSerialPort _serialport;

    kfly_comm::codec _kfly_comm;

    std::mutex _serialmutex;

public:
    explicit communication(QObject *parent = 0);
    ~communication();

    bool openPort(const QString& portname, int baudrate);
    void closePort();

    void send(const std::vector<uint8_t>& message);

    void subscribe(kfly_comm::commands cmd, unsigned int dt_ms);
    void unsubscribe(kfly_comm::commands cmd);
    void unsubscribe_all();

    /* Functions registered to the KFly interface. */
    void regPing(kfly_comm::datagrams::Ping);
    void regSystemStrings(kfly_comm::datagrams::SystemStrings msg);
    void regSystemStatus(kfly_comm::datagrams::SystemStatus msg);
    void regRCInputSettings(kfly_comm::datagrams::RCInputSettings msg);
    void regRCValues(kfly_comm::datagrams::RCValues msg);
    void regRCOutputSettings(kfly_comm::datagrams::RCOutputSettings msg);
    void regControlSignals(kfly_comm::datagrams::ControlSignals msg);
    void regChannelMix(kfly_comm::datagrams::ChannelMix msg);
    void regArmSettings(kfly_comm::datagrams::ArmSettings msg);
    void regRateControllerData(kfly_comm::datagrams::RateControllerData msg);
    void regAttitudeControllerData(kfly_comm::datagrams::AttitudeControllerData msg);
    void regControllerLimits(kfly_comm::datagrams::ControllerLimits msg);
    void regIMUCalibration(kfly_comm::datagrams::IMUCalibration msg);
    void regRawIMUData(kfly_comm::datagrams::RawIMUData msg);
    void regIMUData(kfly_comm::datagrams::IMUData msg);


private slots:
    void parseSerialData();
    void handleSerialError(QSerialPort::SerialPortError error);

signals:
    void sigConnectionError(void);
    void sigPing(void);
    void sigSystemStrings(kfly_comm::datagrams::SystemStrings msg);
    void sigSystemStatus(kfly_comm::datagrams::SystemStatus msg);
    void sigRCInputSettings(kfly_comm::datagrams::RCInputSettings msg);
    void sigRCValues(kfly_comm::datagrams::RCValues msg);
    void sigRCOutputSettings(kfly_comm::datagrams::RCOutputSettings msg);
    void sigControlSignals(kfly_comm::datagrams::ControlSignals msg);
    void sigChannelMix(kfly_comm::datagrams::ChannelMix msg);
    void sigArmSettings(kfly_comm::datagrams::ArmSettings msg);
    void sigRateControllerData(kfly_comm::datagrams::RateControllerData msg);
    void sigAttitudeControllerData(kfly_comm::datagrams::AttitudeControllerData msg);
    void sigControllerLimits(kfly_comm::datagrams::ControllerLimits msg);
    void sigIMUCalibration(kfly_comm::datagrams::IMUCalibration msg);
    void sigRawIMUData(kfly_comm::datagrams::RawIMUData msg);
    void sigIMUData(kfly_comm::datagrams::IMUData msg);

};

#endif // COMMUNICATION_H
