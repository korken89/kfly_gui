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

    kfly_comm::codec _comm;

public:
    explicit communication(QObject *parent = 0);
    ~communication();

    void openPort(QString portname, int baudrate);
    void closePort();

    void send(const std::vector<uint8_t>& message);


private:


private slots:
  void parseSerialData();

};

#endif // COMMUNICATION_H
