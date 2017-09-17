#ifndef UI_SENSORS_H
#define UI_SENSORS_H

#include <QWidget>

namespace Ui {
class ui_sensors;
}

class ui_sensors : public QWidget
{
    Q_OBJECT

public:
    explicit ui_sensors(QWidget *parent = 0);
    ~ui_sensors();

private:
    Ui::ui_sensors *ui;
};

#endif // UI_SENSORS_H
