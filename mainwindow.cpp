#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_connect.h"
#include "ui_sys_info.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), comms(new communication)
{
    ui->setupUi(this);

    ui->widConnect->register_communication(comms);
    ui->tabStatus->register_communication(comms);

    //ui->tabWidget->setEnabled(false);
    ui->buttonSave->setEnabled(false);

    connect(ui->widConnect, &ui_connect::heartbeat,
            this, &MainWindow::heartbeat);

    connect(ui->widConnect, &ui_connect::connection_lost,
            this, &MainWindow::connection_lost);

    connect(ui->widConnect, &ui_connect::heartbeat,
            ui->tabStatus, &ui_sys_info::heartbeat);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete comms;
}

void MainWindow::heartbeat()
{
    //qDebug() << "Got heartbeat!!!";
    //ui->tabWidget->setEnabled(true);
    ui->buttonSave->setEnabled(true);
}

void MainWindow::connection_lost()
{
    qDebug() << "Got connection lost!!!";
    //ui->tabWidget->setEnabled(false);
    ui->buttonSave->setEnabled(false);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();

    //if (QMessageBox::Yes == QMessageBox::question(this,
    //                                              "Close Confirmation",
    //                                              "Exit without saving changes to the board?",
    //                                              QMessageBox::Yes |
    //                                              QMessageBox::No))
    //{
        event->accept();
    //}
}
