#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), comms(new communication)
{
    ui->setupUi(this);

    ui->widConnect->register_communication(comms);
    ui->tabStatus->register_communication(comms);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete comms;
}
