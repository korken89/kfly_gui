#include "ui_sensors.h"
#include "ui_ui_sensors.h"

ui_sensors::ui_sensors(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_sensors),
    calibrator_(1000)
{
    ui->setupUi(this);

    _auto_upload_checked = false;
    _upload_settings = false;
}

ui_sensors::~ui_sensors()
{
    delete ui;
}

void ui_sensors::register_communication(communication *com)
{
    _communication = com;

    connect(_communication, &communication::sigIMUCalibration,
            this, &ui_sensors::imu_calibration);

    connect(_communication, &communication::sigRawIMUData,
            this, &ui_sensors::imu_rawdata);

    connect(&_upload_settings_timer, &QTimer::timeout,
            this, &ui_sensors::upload_settings_timer);

    _upload_settings_timer.start(1000);
}

void ui_sensors::showEvent(QShowEvent *)
{
}

void ui_sensors::hideEvent(QHideEvent *)
{
    if (_communication != nullptr)
        _communication->unsubscribe(kfly_comm::commands::GetRawIMUData);
}

void ui_sensors::upload_settings()
{
    qDebug() << "imu calibration upload settings";

    kfly_comm::datagrams::IMUCalibration msg;

    // TODO: Implement this

    // if (_communication != nullptr)
    //     _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_sensors::connection_established()
{
    /* Request on connection. */
    if (_communication != nullptr)
    {
        using namespace kfly_comm;
        _communication->send(codec::generate_command(commands::GetIMUCalibration));
    }
}

void ui_sensors::auto_upload_changed(bool checked)
{
    _auto_upload_checked = checked;
}

void ui_sensors::upload_now()
{
    upload_settings();
}

void ui_sensors::imu_calibration(kfly_comm::datagrams::IMUCalibration msg)
{
    qDebug() << "got imu calibration";

    //TODO: Implement this

}

void ui_sensors::imu_rawdata(kfly_comm::datagrams::RawIMUData msg)
{

    static int i = 0;

    if (i++ == 100)
    {
        qDebug() << "got raw imu data x100";
        i = 0;
    }

    auto state = calibrator_.add_sample(msg.accelerometer[0],
                                        msg.accelerometer[1],
                                        msg.accelerometer[2]);

    if (state == calibration::state::GETTING_SAMPLES)
    {
        ui->progressAcc->setValue(calibrator_.get_axis_percent_done());
    }
    else
    {
        if (_communication != nullptr)
            _communication->unsubscribe(kfly_comm::commands::GetRawIMUData);

        if (calibrator_.axis_finished() == calibration::state::DONE)
        {
            ui->buttonCalibrateAcc->setText("Calibrate");

            // TODO write out calibration + sanity check
        }
        else
            ui->buttonNextAxisAcc->setEnabled(true);

        ui->buttonNextAxisAcc->setText("Next axis");
    }

}

void ui_sensors::upload_settings_timer()
{
    if (_upload_settings && _auto_upload_checked)
    {
        _upload_settings = false;
        upload_settings();
    }
}

void ui_sensors::on_buttonCalibrateAcc_clicked()
{
    if (ui->buttonCalibrateAcc->text() == "Calibrate")
    {
        calibrator_.reset();
        ui->buttonCalibrateAcc->setText("Abort");
        ui->buttonNextAxisAcc->setEnabled(false);
        ui->buttonNextAxisAcc->setText("Sampling...");

        if (_communication != nullptr)
            _communication->subscribe(kfly_comm::commands::GetRawIMUData, 6);
    }
    else
    {
        if (_communication != nullptr)
            _communication->unsubscribe(kfly_comm::commands::GetRawIMUData);

        calibrator_.reset();
        ui->progressAcc->setValue(0);
        ui->buttonNextAxisAcc->setText("Next axis");
        ui->buttonNextAxisAcc->setEnabled(false);
        ui->buttonCalibrateAcc->setText("Calibrate");
    }
}

void ui_sensors::on_buttonNextAxisAcc_clicked()
{
    if (_communication != nullptr)
        _communication->subscribe(kfly_comm::commands::GetRawIMUData, 6);

    ui->buttonNextAxisAcc->setEnabled(false);
    ui->buttonNextAxisAcc->setText("Sampling...");
}
