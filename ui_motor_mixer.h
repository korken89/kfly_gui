#ifndef UI_MOTOR_MIXER_H
#define UI_MOTOR_MIXER_H

#include <QWidget>
#include <QTimer>
#include <vector>

#include "communication.h"
#include "ui_motor_mixer_channel.h"

namespace Ui {
    class ui_motor_mixer;
}

class ui_motor_mixer : public QWidget
{
    Q_OBJECT

public:
    explicit ui_motor_mixer(QWidget *parent = 0);
    ~ui_motor_mixer();
    void register_communication(communication *com);
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;

private:
    Ui::ui_motor_mixer *ui;

    communication *_communication;
    std::vector<ui_motor_mixer_channel *> _channels;

    bool _auto_upload_checked;
    bool _upload_settings;
    QTimer _upload_settings_timer;

    void upload_settings();

public slots:
    void connection_established();

    void auto_upload_changed(bool checked);
    void upload_now();

private slots:
    void rc_mixer_settings(kfly_comm::datagrams::ChannelMix msg);
    void upload_settings_timer();
    void channel_value_changed();
};

#endif // UI_MOTOR_MIXER_H
