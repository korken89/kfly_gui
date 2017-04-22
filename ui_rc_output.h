#ifndef UI_RC_OUTPUT_H
#define UI_RC_OUTPUT_H

#include <QWidget>

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
};

#endif // UI_RC_OUTPUT_H
