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
    void arming_settings(kfly_comm::datagrams::ArmSettings msg);
    void upload_settings_timer();

    void on_checkAutomaticDisarm_toggled(bool checked);
    void on_checkSpinWhenArmed_toggled(bool checked);

private:
    Ui::ui_arming *ui;
};

#endif // UI_ARMING_H
