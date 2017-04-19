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

    connect(_communication, &communication::sigSystemInformation,
            this, &ui_sys_info::system_information);
}

void ui_sys_info::heartbeat()
{
    /* Request system information on heartbeat. */
    if (!this->isHidden())
    {
        using namespace kfly_comm;
        auto data = codec::generate_command(commands::GetSystemInformation);
        _communication->send(data);
    }
}

void ui_sys_info::system_information(kfly_comm::datagrams::SystemInformation msg)
{
    (void)msg;

    qDebug() << "sys info info";
}
