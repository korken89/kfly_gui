#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_connect.h"
#include "ui_sys_info.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), comms(new communication)
{
    ui->setupUi(this);


    //ui->tabWidget->setEnabled(false);
    //ui->buttonSave->setEnabled(false);

    // Main window connections
    ui->widConnect->register_communication(comms);

    connect(ui->widConnect, &ui_connect::heartbeat,
            this, &MainWindow::connection_established);

    connect(ui->widConnect, &ui_connect::connection_lost,
            this, &MainWindow::connection_lost);

    // Status connections
    ui->tabStatus->register_communication(comms);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabStatus, &ui_sys_info::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabStatus, &ui_sys_info::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabStatus, &ui_sys_info::upload_now);

    // Sensors connections
    ui->tabSensors->register_communication(comms);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabSensors, &ui_sensors::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabSensors, &ui_sensors::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabSensors, &ui_sensors::upload_now);

    // RC input connections
    ui->tabInput->register_communication(comms);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabInput, &ui_rc_input::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabInput, &ui_rc_input::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabInput, &ui_rc_input::upload_now);

    // RC output connections
    ui->tabOutput->register_communication(comms);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabOutput, &ui_rc_output::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabOutput, &ui_rc_output::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabOutput, &ui_rc_output::upload_now);

    // Mixer connections
    ui->tabMixer->register_communication(comms);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabMixer, &ui_motor_mixer::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabMixer, &ui_motor_mixer::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabMixer, &ui_motor_mixer::upload_now);

    // Arming connections
    ui->tabArming->register_communication(comms);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabArming, &ui_arming::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabArming, &ui_arming::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabArming, &ui_arming::upload_now);

    // Controller connections
    ui->tabControllers->register_communication(comms);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabControllers, &ui_controller::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabControllers, &ui_controller::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabControllers, &ui_controller::upload_now);

    // Developer Options connections
    ui->tabDeveloper->register_communication(comms);

    connect(ui->widConnect, &ui_connect::connection_established,
            ui->tabDeveloper, &ui_developer::connection_established);

    connect(ui->widConnect, &ui_connect::auto_upload_changed,
            ui->tabDeveloper, &ui_developer::auto_upload_changed);

    connect(ui->widConnect, &ui_connect::upload_now,
            ui->tabDeveloper, &ui_developer::upload_now);
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
}

void MainWindow::connection_lost()
{
    qDebug() << "Got connection lost!!!";
    //ui->tabWidget->setEnabled(false);
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
