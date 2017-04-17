#ifndef UI_CONNECT_H
#define UI_CONNECT_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "kfly_comm/kfly_comm.h"
#include "communication.h"

namespace Ui {
class ui_connect;
}

class ui_connect : public QWidget
{
    Q_OBJECT

public:

    explicit ui_connect(QWidget *parent = 0);
    ~ui_connect();
    void register_communication(communication *com);

private slots:
    void on_buttonConnect_clicked();

private:
    Ui::ui_connect *ui;
    communication *_communication;

};

#endif // UI_CONNECT_H
