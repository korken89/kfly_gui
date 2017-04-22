#ifndef UI_RC_OUTPUT_CHANNEL_H
#define UI_RC_OUTPUT_CHANNEL_H

#include <QWidget>

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

private:
    Ui::ui_rc_output_channel *ui;

signals:
    void value_changed();
};

#endif // UI_RC_OUTPUT_CHANNEL_H
