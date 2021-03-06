#ifndef UI_ARMING_H
#define UI_ARMING_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include "communication.h"

namespace Ui {
class ui_arming;
}

class ui_arming : public QWidget
{
    Q_OBJECT

public:
    explicit ui_arming(QWidget *parent = 0);
    ~ui_arming();
    void register_communication(communication *com);
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;

private:
    Ui::ui_arming *ui;

    communication *_communication;
    bool _auto_upload_checked;
    bool _upload_settings;
    QTimer _upload_settings_timer;

    void upload_settings();
    kfly_comm::enums::Arming_Stick_Direction arming_direction_to_enum();
    void set_arming_direction(kfly_comm::enums::Arming_Stick_Direction dir);

public slots:
    void connection_established();

    void auto_upload_changed(bool checked);
    void upload_now();

private slots:
    void arming_settings(kfly_comm::datagrams::ArmSettings msg);
    void upload_settings_timer();

    void on_checkAutomaticDisarm_toggled(bool checked);
    void on_checkSpinWhenArmed_toggled(bool checked);

    void on_boxArmAction_currentIndexChanged(int);
    void on_spinArmThreshold_valueChanged(int);
    void on_spinArmTime_valueChanged(double);
    void on_spinAutomaticDisarmTime_valueChanged(int);
    void on_spinSpinWhenArmedThrottle_valueChanged(int);
};

#endif // UI_ARMING_H
