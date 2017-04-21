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

    void subscribe(kfly_comm::commands cmd, unsigned int dt_ms);
    void unsubscribe(kfly_comm::commands cmd);
    void unsubscribe_all();

    /* Functions registered to the KFly interface. */
    void regPing(kfly_comm::datagrams::Ping);
    void regSystemStrings(kfly_comm::datagrams::SystemStrings msg);
    void regSystemStatus(kfly_comm::datagrams::SystemStatus msg);

private slots:
    void parseSerialData();
    void handleSerialError(QSerialPort::SerialPortError error);

signals:
    void sigConnectionError(void);
    void sigPing(void);
    void sigSystemStrings(kfly_comm::datagrams::SystemStrings msg);
    void sigSystemStatus(kfly_comm::datagrams::SystemStatus msg);

};

#endif // COMMUNICATION_H
