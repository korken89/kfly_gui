#include "ui_rc_output.h"
#include "ui_ui_rc_output.h"

ui_rc_output::ui_rc_output(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_rc_output)
{
    ui->setupUi(this);
}

ui_rc_output::~ui_rc_output()
{
    delete ui;
}
