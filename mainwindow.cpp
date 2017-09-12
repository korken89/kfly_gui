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
    ui->tabMixer->register_communication(comms);

    //ui->tabWidget->setEnabled(false);
    //ui->buttonSave->setEnabled(false);

    // Main window connections
    connect(ui->widConnect, &ui_connect::heartbeat,
            this, &MainWindow::connection_established);

    connect(ui->widConnect, &ui_connect::connection_lost,
            this, &MainWindow::connection_lost);

    // Status connections
    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabStatus, &ui_sys_info::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabStatus, &ui_sys_info::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabStatus, &ui_sys_info::upload_now);

    // RC input connections
    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabInput, &ui_rc_input::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabInput, &ui_rc_input::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabInput, &ui_rc_input::upload_now);

    // RC output connections
    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabOutput, &ui_rc_output::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabOutput, &ui_rc_output::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabOutput, &ui_rc_output::upload_now);

    // Mixer connections
    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabMixer, &ui_motor_mixer::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabMixer, &ui_motor_mixer::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabMixer, &ui_motor_mixer::upload_now);
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
    //ui->buttonSave->setEnabled(true);
}

void MainWindow::connection_lost()
{
    qDebug() << "Got connection lost!!!";
    //ui->tabWidget->setEnabled(false);
    //ui->buttonSave->setEnabled(false);
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
