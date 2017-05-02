#ifndef UI_RC_OUTPUT_CHANNEL_H
#define UI_RC_OUTPUT_CHANNEL_H

#include <QWidget>
#include <QComboBox>
#include <QSlider>
#include <QProgressBar>
#include "communication.h"

namespace Ui {
class ui_rc_output_channel;
}

class ui_rc_output_channel : public QWidget
{
    Q_OBJECT

public:
    explicit ui_rc_output_channel(QWidget *parent = 0);
    ~ui_rc_output_channel();
    void set_channel_number(int i);
    void set_mode_enabled(bool enabled);
    void set_mode(kfly_comm::enums::RCOutput_Mode mode);
    kfly_comm::enums::RCOutput_Mode get_mode();
    void set_channel_enabled(bool enabled);
    bool get_channel_enabled();
    void set_bar_value(float value);
    void set_override_enabled(bool enabled);
    void set_override_value(int val);
    int get_override_value();

private:
    Ui::ui_rc_output_channel *ui;

signals:
    void value_changed();

private slots:
    void on_buttonEnable_toggled(bool checked);
    void on_boxOutputMode_currentTextChanged(const QString &arg1);
    void on_sliderSetValue_sliderMoved(int position);
};

#endif // UI_RC_OUTPUT_CHANNEL_H
