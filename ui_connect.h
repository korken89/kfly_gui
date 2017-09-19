#ifndef UI_CONNECT_H
#define UI_CONNECT_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
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

private:
    Ui::ui_connect *ui;
    communication *_communication;

    void connect_port();
    void disconnect_port();

    QTimer ping_timer;
    int ping_counter = 0;
    bool first_ping;
    void ping_timer_event();

    QTimer check_port_timer;
    void check_port_timer_event();

signals:
    void heartbeat();
    void connection_established();
    void connection_lost();
    void auto_upload_changed(bool checked);
    void upload_now();

public slots:
    void ping_received();
    void connection_error();

private slots:
    void on_buttonConnect_clicked();


    void on_buttonAutoUpload_toggled(bool checked);
    void on_buttonApplyChanges_clicked();
};

#endif // UI_CONNECT_H
