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

  void set_channel_number(int i);

  float get_roll_weight();
  void set_roll_weight(float value);
  float get_pitch_weight();
  void set_pitch_weight(float value);
  float get_yaw_weight();
  void set_yaw_weight(float value);
  float get_throttle_weight();
  void set_throttle_weight(float value);
  float get_offset();
  void set_offset(float value);

private:
  Ui::ui_motor_mixer_channel *ui;

signals:
    void value_changed();

private slots:
    void on_rollSpinBox_valueChanged(double);
    void on_pitchSpinBox_valueChanged(double);
    void on_yawSpinBox_valueChanged(double);
    void on_throttleSpinBox_valueChanged(double);
    void on_offsetSpinBox_valueChanged(double);
};

#endif // UI_MOTOR_MIXER_CHANNEL_H
