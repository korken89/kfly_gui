#ifndef UI_SYS_INFO_H
#define UI_SYS_INFO_H

#include <QWidget>
#include "communication.h"

namespace Ui {
class ui_sys_info;
}

class ui_sys_info : public QWidget
{
    Q_OBJECT

public:
    explicit ui_sys_info(QWidget *parent = 0);
    ~ui_sys_info();
    void register_communication(communication *com);


private:
    Ui::ui_sys_info *ui;
    communication *_communication;

public slots:
    void slotSystemInformation(kfly_comm::datagrams::SystemInformation msg);

};

#endif // UI_SYS_INFO_H
