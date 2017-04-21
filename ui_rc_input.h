#ifndef UI_RC_INPUT_H
#define UI_RC_INPUT_H

#include <QWidget>
#include <QDebug>
#include <vector>
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

private:
    Ui::ui_rc_input *ui;
    std::vector<ui_rc_input_channel *> _channels;

private slots:
    void channel_value_changed();
};

#endif // UI_RC_INPUT_H
