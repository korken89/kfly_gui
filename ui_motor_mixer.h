#ifndef UI_MOTOR_MIXER_H
#define UI_MOTOR_MIXER_H

#include <QWidget>

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
};

#endif // UI_MOTOR_MIXER_H
