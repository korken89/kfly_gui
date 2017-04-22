#include "ui_rc_output_channel.h"
#include "ui_ui_rc_output_channel.h"

ui_rc_output_channel::ui_rc_output_channel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_rc_output_channel)
{
    ui->setupUi(this);
}

ui_rc_output_channel::~ui_rc_output_channel()
{
    delete ui;
}
