#ifndef UI_ARMING_H
#define UI_ARMING_H

#include <QWidget>

namespace Ui {
class ui_arming;
}

class ui_arming : public QWidget
{
    Q_OBJECT

public:
    explicit ui_arming(QWidget *parent = 0);
    ~ui_arming();

private slots:
    void on_checkAutomaticDisarm_toggled(bool checked);

    void on_checkSpinWhenArmed_toggled(bool checked);

private:
    Ui::ui_arming *ui;
};

#endif // UI_ARMING_H
