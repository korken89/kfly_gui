#include "ui_controller.h"
#include "ui_ui_controller.h"

ui_controller::ui_controller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_controller)
{
    ui->setupUi(this);
}

ui_controller::~ui_controller()
{
    delete ui;
}
