#include "ui_rc_input.h"
#include "ui_ui_rc_input.h"

ui_rc_input::ui_rc_input(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_rc_input)
{
    ui->setupUi(this);

    _upload_settings = false;

    // Setup the channels

    for (int i = 0; i < 12; i++)
    {
        ui_rc_input_channel *chan = new ui_rc_input_channel;
        _channels.push_back(chan);

        chan->set_channel_number(i+1);

        /* Connect each channel to know when any value has changed. */
        connect(chan, &ui_rc_input_channel::value_changed,
                this, &ui_rc_input::channel_value_changed);

        ui->layoutChannels->addWidget(chan);
    }

    // Setup input settings
    ui->boxInputType->addItem("PWM");
    ui->boxInputType->addItem("CPPM");
    ui->boxInputType->addItem("SBUS");
}

ui_rc_input::~ui_rc_input()
{
    delete ui;
}

void ui_rc_input::register_communication(communication *com)
{
    _communication = com;

    connect(_communication, &communication::sigRCValues,
            this, &ui_rc_input::rc_values);

    connect(_communication, &communication::sigRCInputSettings,
            this, &ui_rc_input::rc_input_settings);

    connect(&_upload_settings_timer, &QTimer::timeout,
            this, &ui_rc_input::upload_settings_timer);
    _upload_settings_timer.start(1000);
}

void ui_rc_input::showEvent(QShowEvent *)
{
    if (_communication != nullptr)
        _communication->subscribe(kfly_comm::commands::GetRCValues, 100);
}

void ui_rc_input::hideEvent(QHideEvent *)
{
    if (_communication != nullptr)
        _communication->unsubscribe(kfly_comm::commands::GetRCValues);
}

void ui_rc_input::upload_settings()
{
    qDebug() << "rc input upload settings";

    kfly_comm::datagrams::RCInputSettings msg;

    for (unsigned i = 0; i < _channels.size(); i++)
    {
        msg.ch_top[i] = _channels[i]->get_maximum();
        msg.ch_center[i] = _channels[i]->get_center();
        msg.ch_bottom[i] = _channels[i]->get_minimum();
        msg.ch_reverse[i] = _channels[i]->get_inverted();
        msg.role[i] = _channels[i]->get_role();
        msg.type[i] = _channels[i]->get_type();
    }

    if (ui->boxInputType->currentText() == "PWM")
        msg.mode = kfly_comm::enums::RCInput_Mode::MODE_PWM_INPUT;
    else if (ui->boxInputType->currentText() == "CPPM")
        msg.mode = kfly_comm::enums::RCInput_Mode::MODE_CPPM_INPUT;
    else if (ui->boxInputType->currentText() == "SBUS") // For future use
        msg.mode = kfly_comm::enums::RCInput_Mode::MODE_CPPM_INPUT;
    else
        msg.mode = kfly_comm::enums::RCInput_Mode::MODE_CPPM_INPUT;

    msg.use_rssi = false;

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_rc_input::connection_established()
{
    ui->buttonStartStopCal->setText("Start Calibration");

    /* Request on connection. */
    if (_communication != nullptr)
    {
        using namespace kfly_comm;
        _communication->send(codec::generate_command(commands::GetRCInputSettings));

        if (!isHidden())
            _communication->subscribe(kfly_comm::commands::GetRCValues, 100);
    }
}

void ui_rc_input::rc_values(kfly_comm::datagrams::RCValues msg)
{
    //qDebug() << "got rc values";

    ui->buttonInputDetected->setChecked(msg.active_connection);
    ui->barRSSI->setValue(msg.rssi);

    for (unsigned i = 0; i < _channels.size(); i++)
    {
        _channels[i]->set_current_value(msg.calibrated_value[i],
                                        msg.channel_value[i]);

        if (ui->buttonStartStopCal->text() == "Stop Calibration")
        {
            if (_start_calibration)
            {
                _channels[i]->set_minimum(msg.channel_value[i]);
                _channels[i]->set_center(msg.channel_value[i]);
                _channels[i]->set_maximum(msg.channel_value[i]);
            }
            else if (_calibrate_centers)
            {
                _channels[i]->set_center(msg.channel_value[i]);
            }
            else
            {
                if (_channels[i]->get_minimum() > msg.channel_value[i])
                    _channels[i]->set_minimum(msg.channel_value[i]);

                if (_channels[i]->get_maximum() < msg.channel_value[i])
                    _channels[i]->set_maximum(msg.channel_value[i]);
            }
        }
    }

    _start_calibration = false;
    _calibrate_centers = false;
}

void ui_rc_input::rc_input_settings(kfly_comm::datagrams::RCInputSettings msg)
{
    qDebug() << "got rc input settings";

    for (unsigned i = 0; i < _channels.size(); i++)
    {
         _channels[i]->set_maximum(msg.ch_top[i]);
         _channels[i]->set_center(msg.ch_center[i]);
         _channels[i]->set_minimum(msg.ch_bottom[i]);
         _channels[i]->set_inverted(msg.ch_reverse[i]);
         _channels[i]->set_role(msg.role[i]);
         _channels[i]->set_type(msg.type[i]);
    }

    if (msg.mode == kfly_comm::enums::RCInput_Mode::MODE_PWM_INPUT)
        ui->boxInputType->setCurrentText("PWM");
    else if (msg.mode == kfly_comm::enums::RCInput_Mode::MODE_CPPM_INPUT)
        ui->boxInputType->setCurrentText("CPPM");
}

void ui_rc_input::upload_settings_timer()
{
    if (_upload_settings && ui->buttonAutoUpload->isChecked())
    {
        _upload_settings = false;
        upload_settings();
    }
}

void ui_rc_input::channel_value_changed()
{
    if (!isHidden())
        _upload_settings = true;
}

void ui_rc_input::on_buttonAutoUpload_toggled(bool checked)
{
    ui->buttonApplyChanges->setEnabled(!checked);
}

void ui_rc_input::on_boxInputType_editTextChanged(const QString &)
{
    if (!isHidden())
        _upload_settings = true;
}

void ui_rc_input::on_buttonApplyChanges_clicked()
{
    upload_settings();
}

void ui_rc_input::on_boxInputType_currentTextChanged(const QString &)
{
    if (!isHidden())
        _upload_settings = true;
}

void ui_rc_input::on_buttonStartStopCal_clicked()
{
    if (ui->buttonStartStopCal->text() == "Start Calibration")
    {
        ui->buttonStartStopCal->setText("Stop Calibration");
        ui->buttonCenterCal->setEnabled(true);
        ui->buttonAutoUpload->setChecked(false);
        _start_calibration = true;
        _calibrate_centers = false;
    }
    else
    {
        ui->buttonStartStopCal->setText("Start Calibration");
        ui->buttonCenterCal->setEnabled(false);
        _calibrate_centers = false;
    }
}

void ui_rc_input::on_buttonCenterCal_clicked()
{
     _calibrate_centers = true;
}
