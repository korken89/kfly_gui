#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

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

public slots:
    void connection_established();

    void auto_upload_changed(bool checked);
    void upload_now();

private slots:
    void rate_controller_settings(kfly_comm::datagrams::RateControllerData msg);
    void attitude_controller_settings(kfly_comm::datagrams::AttitudeControllerData msg);
    void limits_settings(kfly_comm::datagrams::ControllerLimits msg);
    void upload_settings_timer();
};

#endif // UI_CONTROLLER_H
