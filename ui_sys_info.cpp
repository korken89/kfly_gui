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
