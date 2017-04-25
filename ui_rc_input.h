#ifndef UI_RC_INPUT_H
#define UI_RC_INPUT_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <vector>
#include "communication.h"
#include "ui_rc_input_channel.h"

namespace Ui {
class ui_rc_input;
}

class ui_rc_input : public QWidget
{
    Q_OBJECT

public:
    explicit ui_rc_input(QWidget *parent = 0);
    ~ui_rc_input();
    void register_communication(communication *com);
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;

private:
    Ui::ui_rc_input *ui;
    communication *_communication;
    std::vector<ui_rc_input_channel *> _channels;
    bool _upload_settings;
    QTimer _upload_settings_timer;

    bool _start_calibration;
    bool _calibrate_centers;
    void upload_settings();

public slots:
    void connection_established();

private slots:
    void rc_values(kfly_comm::datagrams::RCValues msg);
    void rc_input_settings(kfly_comm::datagrams::RCInputSettings msg);
    void upload_settings_timer();
    void channel_value_changed();
    void on_buttonAutoUpload_toggled(bool checked);
    void on_boxInputType_editTextChanged(const QString &);
    void on_buttonApplyChanges_clicked();
    void on_boxInputType_currentTextChanged(const QString &);
    void on_buttonStartStopCal_clicked();
    void on_buttonCenterCal_clicked();
};

#endif // UI_RC_INPUT_H
