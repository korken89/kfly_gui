#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include <cmath>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include "communication.h"

namespace Ui {
class ui_controller;
}

class ui_controller : public QWidget
{
    Q_OBJECT

public:
    explicit ui_controller(QWidget *parent = 0);
    ~ui_controller();
    void register_communication(communication *com);
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;

private:
    Ui::ui_controller *ui;

    communication *_communication;
    bool _auto_upload_checked;
    bool _upload_rate_controller_settings;
    bool _upload_attitude_controller_settings;
    bool _upload_limit_settings;
    QTimer _upload_settings_timer;

    void upload_rate_controller_settings();
    void upload_attitude_controller_settings();
    void upload_limits_settings();

    double to_radians(double in);
    double to_degrees(double in);
    void update_rate_plot();

public slots:
    void connection_established();

    void auto_upload_changed(bool checked);
    void upload_now();

private slots:
    void rate_controller_settings(kfly_comm::datagrams::RateControllerData msg);
    void attitude_controller_settings(kfly_comm::datagrams::AttitudeControllerData msg);
    void limits_settings(kfly_comm::datagrams::ControllerLimits msg);
    void control_filter_settings(kfly_comm::datagrams::ControlFilterSettings msg);
    void upload_settings_timer();

    void on_spinRateRoll_p_valueChanged(double);
    void on_spinRateRoll_i_valueChanged(double);
    void on_spinRatePitch_p_valueChanged(double);
    void on_spinRatePitch_i_valueChanged(double);
    void on_spinRateYaw_p_valueChanged(double);
    void on_spinRateYaw_i_valueChanged(double);

    void on_spinAttitudeRoll_p_valueChanged(double);
    void on_spinAttitudeRoll_i_valueChanged(double);
    void on_spinAttitudePitch_p_valueChanged(double);
    void on_spinAttitudePitch_i_valueChanged(double);

    void on_spinLimitRateRoll_valueChanged(int);
    void on_spinLimitRatePitch_valueChanged(int);
    void on_spinLimitRateYaw_valueChanged(int);
    void on_spinCenterRateRoll_valueChanged(int);
    void on_spinCenterRatePitch_valueChanged(int);
    void on_spinCenterRateYaw_valueChanged(int);
    void on_spinLimitAttitudeRoll_valueChanged(int);
    void on_spinLimitAttitudePitch_valueChanged(int);
};

#endif // UI_CONTROLLER_H
