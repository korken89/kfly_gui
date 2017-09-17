#ifndef UI_SENSORS_H
#define UI_SENSORS_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include "communication.h"

namespace Ui {
class ui_sensors;
}

class ui_sensors : public QWidget
{
    Q_OBJECT

public:
    explicit ui_sensors(QWidget *parent = 0);
    ~ui_sensors();
    void register_communication(communication *com);
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;

private:
    Ui::ui_sensors *ui;

    communication *_communication;
    bool _auto_upload_checked;
    bool _upload_settings;
    QTimer _upload_settings_timer;

    void upload_settings();

public slots:
    void connection_established();

    void auto_upload_changed(bool checked);
    void upload_now();

private slots:
    void imu_calibration(kfly_comm::datagrams::IMUCalibration msg);
    void imu_rawdata(kfly_comm::datagrams::RawIMUData msg);
    void upload_settings_timer();
};

#endif // UI_SENSORS_H
