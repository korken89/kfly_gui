#ifndef UI_SYS_INFO_H
#define UI_SYS_INFO_H

#include <QWidget>

namespace Ui {
class ui_sys_info;
}

class ui_sys_info : public QWidget
{
    Q_OBJECT

public:
    explicit ui_sys_info(QWidget *parent = 0);
    ~ui_sys_info();

private:
    Ui::ui_sys_info *ui;
};

#endif // UI_SYS_INFO_H
