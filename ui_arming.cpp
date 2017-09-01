#include "ui_arming.h"
#include "ui_ui_arming.h"

ui_arming::ui_arming(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_arming)
{
    ui->setupUi(this);

    ui->boxArmAction->addItem("None");
    ui->boxArmAction->addItem("Non-latching switch");
    ui->boxArmAction->addItem("Pitch min");
    ui->boxArmAction->addItem("Pitch max");
    ui->boxArmAction->addItem("Roll min");
    ui->boxArmAction->addItem("Roll max");
    ui->boxArmAction->addItem("Yaw min");
    ui->boxArmAction->addItem("Yaw max");

    ui->spinAutomaticDisarmTime->setEnabled(false);
    ui->spinSpinWhenArmedThrottle->setEnabled(false);
}

ui_arming::~ui_arming()
{
    delete ui;
}

void ui_arming::on_checkAutomaticDisarm_toggled(bool checked)
{
    ui->spinAutomaticDisarmTime->setEnabled(checked);
}

void ui_arming::on_checkSpinWhenArmed_toggled(bool checked)
{
    ui->spinSpinWhenArmedThrottle->setEnabled(checked);
}
