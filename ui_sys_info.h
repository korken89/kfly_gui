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
    void connection_established();
    void system_strings(kfly_comm::datagrams::SystemStrings msg);
    void system_status(kfly_comm::datagrams::SystemStatus msg);


};

#endif // UI_SYS_INFO_H
