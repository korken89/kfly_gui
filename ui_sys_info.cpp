#include "ui_sys_info.h"
#include "ui_ui_sys_info.h"

ui_sys_info::ui_sys_info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_sys_info)
{
    ui->setupUi(this);
}

ui_sys_info::~ui_sys_info()
{
    delete ui;
}

void ui_sys_info::register_communication(communication *com)
{
    _communication = com;

    connect(com, &communication::sigSystemInformation,
            this, &ui_sys_info::slotSystemInformation);
}

void ui_sys_info::slotSystemInformation(kfly_comm::datagrams::SystemInformation msg)
{
    (void)msg;
}
