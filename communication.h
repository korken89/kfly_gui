#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QtCore>
#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <vector>
#include "kfly_comm/kfly_comm.h"

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

    /* Functions registered to the KFly interface. */
    void regPing(kfly_comm::datagrams::Ping);
    void regSystemInformation(kfly_comm::datagrams::SystemInformation msg);

private slots:
    void parseSerialData();
    void handleSerialError(QSerialPort::SerialPortError error);

signals:
    void sigConnectionError(void);
    void sigPing(void);
    void sigSystemInformation(kfly_comm::datagrams::SystemInformation msg);

};

#endif // COMMUNICATION_H
