#ifndef UI_RC_INPUT_CHANNEL_H
#define UI_RC_INPUT_CHANNEL_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QProgressBar>
#include "communication.h"

namespace Ui {
class ui_rc_input_channel;
}

class ui_rc_input_channel : public QWidget
{
    Q_OBJECT

public:
    explicit ui_rc_input_channel(QWidget *parent = 0);
    ~ui_rc_input_channel();

    void set_channel_number(int i);
    void set_current_value(float cal_value, int raw_value);

    int get_minimum();
    int get_center();
    int get_maximum();

    void set_minimum(int value);
    void set_center(int value);
    void set_maximum(int value);

    kfly_comm::enums::RCInput_Role get_role();
    kfly_comm::enums::RCInput_Type get_type();
    bool get_inverted();

    void set_role(kfly_comm::enums::RCInput_Role role);
    void set_type(kfly_comm::enums::RCInput_Type type);
    void set_inverted(bool inverted);

private:
    Ui::ui_rc_input_channel *ui;

signals:
    void value_changed();

private slots:
    void on_boxRole_currentTextChanged(const QString &text);
    void on_spinBottom_valueChanged(int value);
    void on_spinCenter_valueChanged(int value);
    void on_spinTop_valueChanged(int value);
    void on_boxType_currentTextChanged(const QString &text);
    void on_buttonInvert_toggled(bool checked);
};

#endif // UI_RC_INPUT_CHANNEL_H
