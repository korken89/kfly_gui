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
    ui->tabInput->register_communication(comms);
    ui->tabOutput->register_communication(comms);

    //ui->tabWidget->setEnabled(false);
    ui->buttonSave->setEnabled(false);

    connect(ui->widConnect, &ui_connect::heartbeat,
            this, &MainWindow::connection_established);

    connect(ui->widConnect, &ui_connect::connection_lost,
            this, &MainWindow::connection_lost);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabStatus, &ui_sys_info::connection_established);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabInput, &ui_rc_input::connection_established);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabOutput, &ui_rc_output::connection_established);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete comms;
}

void MainWindow::connection_established()
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
