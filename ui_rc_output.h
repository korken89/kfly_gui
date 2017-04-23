#ifndef UI_RC_OUTPUT_H
#define UI_RC_OUTPUT_H

#include <QWidget>
#include <QDebug>
#include <vector>
#include "ui_rc_output_channel.h"

namespace Ui {
class ui_rc_output;
}

class ui_rc_output : public QWidget
{
    Q_OBJECT

public:
    explicit ui_rc_output(QWidget *parent = 0);
    ~ui_rc_output();

private:
    Ui::ui_rc_output *ui;
    std::vector<ui_rc_output_channel *> _channels;

private slots:
    void channel_value_changed();
    void on_buttonAutoUpload_toggled(bool checked);
};

#endif // UI_RC_OUTPUT_H
