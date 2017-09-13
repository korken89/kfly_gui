#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include <QWidget>

namespace Ui {
class ui_controller;
}

class ui_controller : public QWidget
{
    Q_OBJECT

public:
    explicit ui_controller(QWidget *parent = 0);
    ~ui_controller();

private:
    Ui::ui_controller *ui;
};

#endif // UI_CONTROLLER_H
