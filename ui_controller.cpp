#include "ui_controller.h"
#include "ui_ui_controller.h"

ui_controller::ui_controller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_controller)
{
    ui->setupUi(this);

    ui->spinCenterRateRoll->setValue(100);
    ui->spinCenterRatePitch->setValue(100);
    ui->spinCenterRateYaw->setValue(100);

    ui->spinLimitRateRoll->setValue(1000);
    ui->spinLimitRatePitch->setValue(1000);
    ui->spinLimitRateYaw->setValue(600);

    ui->plotRate->plotLayout()->insertRow(0);
    ui->plotRate->plotLayout()->addElement(0, 0,
        new QCPTextElement(ui->plotRate, "Rate curves vs. Stick input"));
    ui->plotRate->legend->setVisible(true);
    ui->plotRate->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
    update_rate_plot();

    _auto_upload_checked = false;
    _upload_rate_controller_settings = false;
    _upload_attitude_controller_settings = false;
    _upload_limit_settings = false;
}

ui_controller::~ui_controller()
{
    delete ui;
}

void ui_controller::register_communication(communication *com)
{
    _communication = com;

    connect(_communication, &communication::sigRateControllerData,
            this, &ui_controller::rate_controller_settings);

    connect(_communication, &communication::sigAttitudeControllerData,
            this, &ui_controller::attitude_controller_settings);

    connect(_communication, &communication::sigControllerLimits,
            this, &ui_controller::limits_settings);

    connect(_communication, &communication::sigControlFilterSettings,
            this, &ui_controller::control_filter_settings);

    connect(&_upload_settings_timer, &QTimer::timeout,
            this, &ui_controller::upload_settings_timer);

    _upload_settings_timer.start(1000);
}

void ui_controller::showEvent(QShowEvent *)
{
}

void ui_controller::hideEvent(QHideEvent *)
{
}

void ui_controller::upload_rate_controller_settings()
{
    qDebug() << "rate controller upload settings";

    kfly_comm::datagrams::RateControllerData msg;

    msg.roll_controller.P_gain = ui->spinRateRoll_p->value();
    msg.roll_controller.I_gain = ui->spinRateRoll_i->value();

    msg.pitch_controller.P_gain = ui->spinRatePitch_p->value();
    msg.pitch_controller.I_gain = ui->spinRatePitch_i->value();

    msg.yaw_controller.P_gain = ui->spinRateYaw_p->value();
    msg.yaw_controller.I_gain = ui->spinRateYaw_i->value();

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_controller::upload_attitude_controller_settings()
{
    qDebug() << "attitude controller upload settings";

    kfly_comm::datagrams::AttitudeControllerData msg;

    msg.roll_controller.P_gain = ui->spinAttitudeRoll_p->value();
    msg.roll_controller.I_gain = 0;

    msg.pitch_controller.P_gain = ui->spinAttitudePitch_p->value();
    msg.pitch_controller.I_gain = 0;

    // TODO: For future use
    msg.yaw_controller.P_gain = 0;
    msg.yaw_controller.I_gain = 0;

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_controller::upload_limits_settings()
{
    qDebug() << "controller limits upload settings";

    kfly_comm::datagrams::ControllerLimits msg;

    msg.max_rate.max_rate.roll = to_radians(ui->spinLimitRateRoll->value());
    msg.max_rate.max_rate.pitch = to_radians(ui->spinLimitRatePitch->value());
    msg.max_rate.max_rate.yaw = to_radians(ui->spinLimitRateYaw->value());

    msg.max_rate.center_rate.roll = to_radians(ui->spinCenterRateRoll->value());
    msg.max_rate.center_rate.pitch = to_radians(ui->spinCenterRatePitch->value());
    msg.max_rate.center_rate.yaw = to_radians(ui->spinCenterRateYaw->value());

    msg.max_angle.roll = to_radians(ui->spinLimitAttitudeRoll->value());
    msg.max_angle.pitch = to_radians(ui->spinLimitAttitudePitch->value());

    msg.max_velocity.horizontal = 0;
    msg.max_velocity.vertical = 0;

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

double ui_controller::to_radians(double in)
{
    return (in / 180.0) * M_PI;
}

double ui_controller::to_degrees(double in)
{

    return (in * 180.0) / M_PI;
}

void ui_controller::update_rate_plot()
{
    // generate some data:
    QVector<double> x(101), xp(101), y_roll(101), y_pitch(101), y_yaw(101); // initialize with entries 0..100

    double linear_rate_roll = ui->spinCenterRateRoll->value();
    double max_rate_roll = ui->spinLimitRateRoll->value();
    double linear_rate_pitch = ui->spinCenterRatePitch->value();
    double max_rate_pitch = ui->spinLimitRatePitch->value();
    double linear_rate_yaw = ui->spinCenterRateYaw->value();
    double max_rate_yaw = ui->spinLimitRateYaw->value();

    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      xp[i] = x[i] * 100; // x goes from -1 to 1

      y_roll[i] = linear_rate_roll * x[i] +
                  (max_rate_roll - linear_rate_roll) * x[i] * x[i] * x[i] * std::abs(x[i]);

      y_pitch[i] = linear_rate_pitch * x[i] +
                   (max_rate_pitch - linear_rate_pitch) * x[i] * x[i] * x[i] * std::abs(x[i]);

      y_yaw[i] = linear_rate_yaw * x[i] +
                 (max_rate_yaw - linear_rate_yaw) * x[i] * x[i] * x[i] * std::abs(x[i]);
    }

    // Add plots
    ui->plotRate->clearGraphs();
    ui->plotRate->addGraph();
    ui->plotRate->graph(0)->setPen(QPen(Qt::blue, 1.5));

    ui->plotRate->addGraph();
    ui->plotRate->graph(1)->setPen(QPen(Qt::red, 1.5));

    ui->plotRate->addGraph();
    ui->plotRate->graph(2)->setPen(QPen(Qt::darkGreen, 1.5));

    // create graph and assign data to it:
    ui->plotRate->graph(0)->setData(xp, y_roll);
    ui->plotRate->graph(1)->setData(xp, y_pitch);
    ui->plotRate->graph(2)->setData(xp, y_yaw);

    // names
    ui->plotRate->graph(0)->setName("Roll rate");
    ui->plotRate->graph(1)->setName("Pitch rate");
    ui->plotRate->graph(2)->setName("Yaw rate");

    // give the axes some labels:

    ui->plotRate->xAxis->setLabel("Stick input (%)");
    ui->plotRate->yAxis->setLabel("Rate (deg/s)");
    // set axes ranges, so we see all data:
    //double max = std::max(std::max(max_rate_roll, max_rate_pitch), max_rate_yaw);
    ui->plotRate->rescaleAxes();

    ui->plotRate->replot();
}

void ui_controller::connection_established()
{
    /* Request on connection. */
    if (_communication != nullptr)
    {
        using namespace kfly_comm;
        _communication->send(codec::generate_command(commands::GetRateControllerData));
        _communication->send(codec::generate_command(commands::GetAttitudeControllerData));
        _communication->send(codec::generate_command(commands::GetControllerLimits));
    }
}

void ui_controller::auto_upload_changed(bool checked)
{
    _auto_upload_checked = checked;
}

void ui_controller::upload_now()
{
    upload_rate_controller_settings();
    upload_attitude_controller_settings();
    upload_limits_settings();
}

void ui_controller::rate_controller_settings(kfly_comm::datagrams::RateControllerData msg)
{
    qDebug() << "got rate controller settings";

    ui->spinRateRoll_p->setValue(msg.roll_controller.P_gain);
    ui->spinRateRoll_i->setValue(msg.roll_controller.I_gain);

    ui->spinRatePitch_p->setValue(msg.pitch_controller.P_gain);
    ui->spinRatePitch_i->setValue(msg.pitch_controller.I_gain);

    ui->spinRateYaw_p->setValue(msg.yaw_controller.P_gain);
    ui->spinRateYaw_i->setValue(msg.yaw_controller.I_gain);

}

void ui_controller::attitude_controller_settings(kfly_comm::datagrams::AttitudeControllerData msg)
{
    qDebug() << "got attitude controller settings";

    ui->spinAttitudeRoll_p->setValue(msg.roll_controller.P_gain);
    //ui->spinAttitudeRoll_i->setValue(msg.roll_controller.I_gain);

    ui->spinAttitudePitch_p->setValue(msg.pitch_controller.P_gain);
    //ui->spinAttitudePitch_i->setValue(msg.pitch_controller.I_gain);
}

void ui_controller::limits_settings(kfly_comm::datagrams::ControllerLimits msg)
{
    qDebug() << "got controller limits settings";

    ui->spinLimitRateRoll->setValue(to_degrees(msg.max_rate.max_rate.roll) + 0.5);
    ui->spinLimitRatePitch->setValue(to_degrees(msg.max_rate.max_rate.pitch) + 0.5);
    ui->spinLimitRateYaw->setValue(to_degrees(msg.max_rate.max_rate.yaw) + 0.5);

    ui->spinCenterRateRoll->setValue(to_degrees(msg.max_rate.center_rate.roll) + 0.5);
    ui->spinCenterRatePitch->setValue(to_degrees(msg.max_rate.center_rate.pitch) + 0.5);
    ui->spinCenterRateYaw->setValue(to_degrees(msg.max_rate.center_rate.yaw) + 0.5);

    ui->spinLimitAttitudeRoll->setValue(to_degrees(msg.max_angle.roll) + 0.5);
    ui->spinLimitAttitudePitch->setValue(to_degrees(msg.max_angle.pitch) + 0.5);

    update_rate_plot();
}

void ui_controller::control_filter_settings(kfly_comm::datagrams::ControlFilterSettings msg)
{
    qDebug() << "got controller filter settings";

    // TODO: implement this

}

void ui_controller::upload_settings_timer()
{
    if (_upload_rate_controller_settings && _auto_upload_checked)
    {
        _upload_rate_controller_settings = false;
        upload_rate_controller_settings();
    }

    if (_upload_attitude_controller_settings && _auto_upload_checked)
    {
        _upload_attitude_controller_settings = false;
        upload_attitude_controller_settings();
    }

    if (_upload_limit_settings && _auto_upload_checked)
    {
        _upload_limit_settings = false;
        upload_limits_settings();
    }
}

//
// Changes to rate controller gains
//
void ui_controller::on_spinRateRoll_p_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}

void ui_controller::on_spinRateRoll_i_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}

void ui_controller::on_spinRatePitch_p_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}

void ui_controller::on_spinRatePitch_i_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}

void ui_controller::on_spinRateYaw_p_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}

void ui_controller::on_spinRateYaw_i_valueChanged(double)
{
    if (!isHidden())
        _upload_rate_controller_settings = true;
}



//
// Changes to attitude controller gains
//

void ui_controller::on_spinAttitudeRoll_p_valueChanged(double)
{
    if (!isHidden())
        _upload_attitude_controller_settings = true;
}

void ui_controller::on_spinAttitudeRoll_i_valueChanged(double)
{
    if (!isHidden())
        _upload_attitude_controller_settings = true;
}

void ui_controller::on_spinAttitudePitch_p_valueChanged(double)
{
    if (!isHidden())
        _upload_attitude_controller_settings = true;
}

void ui_controller::on_spinAttitudePitch_i_valueChanged(double)
{
    if (!isHidden())
        _upload_attitude_controller_settings = true;
}



//
// Changes to limits
//
void ui_controller::on_spinLimitRateRoll_valueChanged(int)
{
    if (ui->spinLimitRateRoll->value() < ui->spinCenterRateRoll->value())
        ui->spinCenterRateRoll->setValue(ui->spinLimitRateRoll->value());

    if (!isHidden())
        _upload_limit_settings = true;

    update_rate_plot();
}

void ui_controller::on_spinLimitRatePitch_valueChanged(int)
{
    if (ui->spinLimitRatePitch->value() < ui->spinCenterRatePitch->value())
        ui->spinCenterRatePitch->setValue(ui->spinLimitRatePitch->value());

    if (!isHidden())
        _upload_limit_settings = true;

    update_rate_plot();
}

void ui_controller::on_spinLimitRateYaw_valueChanged(int)
{
    if (ui->spinLimitRateYaw->value() < ui->spinCenterRateYaw->value())
        ui->spinCenterRateYaw->setValue(ui->spinLimitRateYaw->value());

    if (!isHidden())
        _upload_limit_settings = true;

    update_rate_plot();
}

void ui_controller::on_spinCenterRateRoll_valueChanged(int)
{
    if (ui->spinLimitRateRoll->value() < ui->spinCenterRateRoll->value())
        ui->spinLimitRateRoll->setValue(ui->spinCenterRateRoll->value());

    if (!isHidden())
        _upload_limit_settings = true;

    update_rate_plot();
}

void ui_controller::on_spinCenterRatePitch_valueChanged(int)
{
    if (ui->spinLimitRatePitch->value() < ui->spinCenterRatePitch->value())
        ui->spinLimitRatePitch->setValue(ui->spinCenterRatePitch->value());

    if (!isHidden())
        _upload_limit_settings = true;

    update_rate_plot();
}

void ui_controller::on_spinCenterRateYaw_valueChanged(int)
{
    if (ui->spinLimitRateYaw->value() < ui->spinCenterRateYaw->value())
        ui->spinLimitRateYaw->setValue(ui->spinCenterRateYaw->value());

    if (!isHidden())
        _upload_limit_settings = true;

    update_rate_plot();
}

void ui_controller::on_spinLimitAttitudeRoll_valueChanged(int)
{
    if (!isHidden())
        _upload_limit_settings = true;
}

void ui_controller::on_spinLimitAttitudePitch_valueChanged(int)
{
    if (!isHidden())
        _upload_limit_settings = true;
}
