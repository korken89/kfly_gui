#include "ui_developer.h"
#include "ui_ui_developer.h"

ui_developer::ui_developer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_developer)
{
    ui->setupUi(this);
}

ui_developer::~ui_developer()
{
    delete ui;
}
