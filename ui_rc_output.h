#ifndef UI_RC_OUTPUT_H
#define UI_RC_OUTPUT_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <vector>
#include "communication.h"
#include "ui_rc_output_channel.h"

namespace Ui {
class ui_rc_output;
}

class ui_rc_output : public QWidget
{
    Q_OBJECT

public:
    explicit ui_rc_output(QWidget *parent = 0);
    ~ui_rc_output();
    void register_communication(communication *com);
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;

private:
    Ui::ui_rc_output *ui;
    communication *_communication;
    std::vector<ui_rc_output_channel *> _channels;

    bool _auto_upload_checked;
    bool _upload_settings;
    QTimer _upload_settings_timer;

    void upload_settings();
    void upload_override();
    bool _override_enabled;

public slots:
    void connection_established();

    void auto_upload_changed(bool checked);
    void upload_now();

private slots:
    void rc_output_settings(kfly_comm::datagrams::RCOutputSettings msg);
    void control_signals(kfly_comm::datagrams::ControlSignals msg);
    void upload_settings_timer();
    void channel_value_changed();
    void on_buttonOverrideSafeties_toggled(bool checked);
    void on_buttonStartStop_clicked();
    void on_buttonNext_clicked();
};

#endif // UI_RC_OUTPUT_H
