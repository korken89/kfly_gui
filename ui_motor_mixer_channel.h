#ifndef UI_MOTOR_MIXER_CHANNEL_H
#define UI_MOTOR_MIXER_CHANNEL_H

#include <QWidget>

namespace Ui {
  class ui_motor_mixer_channel;
}

class ui_motor_mixer_channel : public QWidget
{
  Q_OBJECT

public:
  explicit ui_motor_mixer_channel(QWidget *parent = 0);
  ~ui_motor_mixer_channel();

private:
  Ui::ui_motor_mixer_channel *ui;
};

#endif // UI_MOTOR_MIXER_CHANNEL_H
