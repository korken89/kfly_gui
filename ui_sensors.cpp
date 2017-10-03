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
    _accept_measurements = false;
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
    if (calibrator_.valid())
    {
        qDebug() << "imu calibration upload settings";

        kfly_comm::datagrams::IMUCalibration msg;

        auto gain = calibrator_.get_gain_result();
        auto bias = calibrator_.get_bias_result();

        for (int i = 0; i < 3; i++)
        {
            msg.accelerometer_gain[i] = gain[i];
            msg.accelerometer_bias[i] = bias[i];
            msg.magnetometer_gain[i] = 1;
            msg.magnetometer_bias[i] = 0;
        }

        msg.timestamp = QDateTime::currentDateTime().toTime_t();

        if (_communication != nullptr)
            _communication->send(kfly_comm::codec::generate_packet(msg));
    }
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

    QString txt = QString::number(1.0 / msg.accelerometer_gain[0], 'g', 6);
    ui->editAccXGain->setText(txt);

    txt = QString::number(1.0 / msg.accelerometer_gain[1], 'g', 6);
    ui->editAccYGain->setText(txt);

    txt = QString::number(1.0 / msg.accelerometer_gain[2], 'g', 6);
    ui->editAccZGain->setText(txt);

    txt = QString::number(msg.accelerometer_bias[0], 'g', 6);
    ui->editAccXBias->setText(txt);

    txt = QString::number(msg.accelerometer_bias[1], 'g', 6);
    ui->editAccYBias->setText(txt);

    txt = QString::number(msg.accelerometer_bias[2], 'g', 6);
    ui->editAccZBias->setText(txt);

    ui->editAccXVariance->setText("N/A");
    ui->editAccYVariance->setText("N/A");
    ui->editAccZVariance->setText("N/A");



    txt = QString::number(1.0 / msg.magnetometer_gain[0], 'g', 6);
    ui->editMagXGain->setText(txt);

    txt = QString::number(1.0 / msg.magnetometer_gain[1], 'g', 6);
    ui->editMagYGain->setText(txt);

    txt = QString::number(1.0 / msg.magnetometer_gain[2], 'g', 6);
    ui->editMagZGain->setText(txt);

    txt = QString::number(msg.magnetometer_bias[0], 'g', 6);
    ui->editMagXBias->setText(txt);

    txt = QString::number(msg.magnetometer_bias[1], 'g', 6);
    ui->editMagYBias->setText(txt);

    txt = QString::number(msg.magnetometer_bias[2], 'g', 6);
    ui->editMagZBias->setText(txt);

    ui->editMagXVariance->setText("N/A");
    ui->editMagYVariance->setText("N/A");
    ui->editMagZVariance->setText("N/A");


    if (msg.timestamp == 0)
        qDebug() << "Warning! IMU not calibrated";

}

void ui_sensors::imu_rawdata(kfly_comm::datagrams::RawIMUData msg)
{

    if (!_accept_measurements)
        return;

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
        _accept_measurements = false;

        if (_communication != nullptr)
            _communication->unsubscribe(kfly_comm::commands::GetRawIMUData);

        ui->progressAcc->setValue(100);

        if (calibrator_.axis_finished() == calibration::state::DONE)
        {
            ui->buttonCalibrateAcc->setText("Calibrate");

            // TODO write out calibration + sanity check
            auto gain = calibrator_.get_gain_result();
            auto bias = calibrator_.get_bias_result();
            auto var = calibrator_.get_variance();

            double nom_gain = 1.0 / ui->editAccGainNominal->text().toDouble();
            double nom_var = ui->editAccVarianceNominal->text().toDouble();

            bool gain_sane = true;
            bool bias_sane = true;
            bool var_sane = true;

            for (int i = 0; i < 3; i++)
            {
                // 10 % of 1g bounds on gain and bias
                gain_sane = gain_sane && (std::abs(gain[i] - nom_gain) < nom_gain * 0.1);
                bias_sane = bias_sane && (std::abs(bias[i]) < 1.0 / nom_gain * 0.1);

                // 3x bounds on variance
                var_sane = var_sane && (std::abs(var[i]) < nom_var * 3);
            }

            QString txt = QString::number(1.0 / gain[0], 'g', 6);
            ui->editAccXGain->setText(txt);

            txt = QString::number(1.0 / gain[1], 'g', 6);
            ui->editAccYGain->setText(txt);

            txt = QString::number(1.0 / gain[2], 'g', 6);
            ui->editAccZGain->setText(txt);


            txt = QString::number(bias[0], 'g', 6);
            ui->editAccXBias->setText(txt);

            txt = QString::number(bias[1], 'g', 6);
            ui->editAccYBias->setText(txt);

            txt = QString::number(bias[2], 'g', 6);
            ui->editAccZBias->setText(txt);


            txt = QString::number(var[0], 'g', 6);
            ui->editAccXVariance->setText(txt);

            txt = QString::number(var[1], 'g', 6);
            ui->editAccYVariance->setText(txt);

            txt = QString::number(var[2], 'g', 6);
            ui->editAccZVariance->setText(txt);


            if (gain_sane && bias_sane && var_sane)
            {
                _upload_settings = true;
            }
            else
            {
                calibrator_.reset();

                if (!gain_sane)
                {
                    QMessageBox* msgBox = new QMessageBox( this );
                    msgBox->setAttribute( Qt::WA_DeleteOnClose ); //makes sure the msgbox is deleted automatically when closed
                    msgBox->setStandardButtons( QMessageBox::Ok );
                    msgBox->setWindowTitle( tr("Error") );
                    msgBox->setText( tr("Gain is not within 10% nominal bounds.") );
                    msgBox->open( );
                }

                if (!bias_sane)
                {
                    QMessageBox* msgBox = new QMessageBox( this );
                    msgBox->setAttribute( Qt::WA_DeleteOnClose ); //makes sure the msgbox is deleted automatically when closed
                    msgBox->setStandardButtons( QMessageBox::Ok );
                    msgBox->setWindowTitle( tr("Error") );
                    msgBox->setText( tr("Bias is not within 10% nominal bounds.") );
                    msgBox->open( );
                }

                if (!var_sane)
                {
                    QMessageBox* msgBox = new QMessageBox( this );
                    msgBox->setAttribute( Qt::WA_DeleteOnClose ); //makes sure the msgbox is deleted automatically when closed
                    msgBox->setStandardButtons( QMessageBox::Ok );
                    msgBox->setWindowTitle( tr("Error") );
                    msgBox->setText( tr("Variance is not within 3x nominal bounds.") );
                    msgBox->open( );
                }

            }
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
        _accept_measurements = true;

        if (_communication != nullptr)
            _communication->subscribe(kfly_comm::commands::GetRawIMUData, 6);
    }
    else
    {
        if (_communication != nullptr)
            _communication->unsubscribe(kfly_comm::commands::GetRawIMUData);

        calibrator_.reset();
        _accept_measurements = false;
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

    _accept_measurements = true;
    ui->buttonNextAxisAcc->setEnabled(false);
    ui->buttonNextAxisAcc->setText("Sampling...");
}
