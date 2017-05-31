#ifndef UI_MOTOR_MIXER_H
#define UI_MOTOR_MIXER_H

#include <QWidget>
#include <QTimer>

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

private:
    Ui::ui_motor_mixer *ui;

    communication *_communication;
    std::vector<ui_motor_mixer_channel *> _channels;

    bool _upload_settings;
    QTimer _upload_settings_timer;

    void upload_settings();
};

#endif // UI_MOTOR_MIXER_H
