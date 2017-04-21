#ifndef UI_RC_INPUT_CHANNEL_H
#define UI_RC_INPUT_CHANNEL_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QProgressBar>

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

    int get_minimum();
    int get_center();
    int get_maximum();

    void set_minimum(int value);
    void set_center(int value);
    void set_maximum(int value);

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
