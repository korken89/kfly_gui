#include "ui_rc_output.h"
#include "ui_ui_rc_output.h"

ui_rc_output::ui_rc_output(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_rc_output)
{
    ui->setupUi(this);

    _override_enabled = false;
    _auto_upload_checked = false;

    for (int i = 0; i < 8; i++)
    {
        ui_rc_output_channel *chan = new ui_rc_output_channel;
        _channels.push_back(chan);

        chan->set_channel_number(i+1);

        if (i == 0 || i == 4)
            chan->set_mode_enabled(true);
        else
            chan->set_mode_enabled(false);

        /* Connect each channel to know when any value has changed. */
        connect(chan, &ui_rc_output_channel::value_changed,
                this, &ui_rc_output::channel_value_changed);

        ui->layoutChannels->addWidget(chan);
    }
}

ui_rc_output::~ui_rc_output()
{
    delete ui;
}

void ui_rc_output::register_communication(communication *com)
{
    _communication = com;

    connect(_communication, &communication::sigControlSignals,
            this, &ui_rc_output::control_signals);

    connect(_communication, &communication::sigRCOutputSettings,
            this, &ui_rc_output::rc_output_settings);

    connect(&_upload_settings_timer, &QTimer::timeout,
            this, &ui_rc_output::upload_settings_timer);
    _upload_settings_timer.start(100);
}

void ui_rc_output::showEvent(QShowEvent *)
{
    if (_communication != nullptr)
        _communication->subscribe(kfly_comm::commands::GetControlSignals, 100);
}

void ui_rc_output::hideEvent(QHideEvent *)
{
    if (_communication != nullptr)
        _communication->unsubscribe(kfly_comm::commands::GetControlSignals);

    ui->buttonOverrideSafeties->setChecked(false);
}

void ui_rc_output::upload_settings()
{
    qDebug() << "uploading rc output settings";

    kfly_comm::datagrams::RCOutputSettings msg;

    for (auto i = 0; i < 8; i++)
        msg.channel_enabled[i] = _channels[i]->get_channel_enabled();

    msg.mode_bank1 = _channels[0]->get_mode();
    msg.mode_bank2 = _channels[4]->get_mode();

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_rc_output::upload_override()
{
    qDebug() << "uploading rc output override";

    kfly_comm::datagrams::MotorOverride msg;

    for (auto i = 0; i < 8; i++)
        msg.values[i] = static_cast<float>(_channels[i]->get_override_value()) / 100.0f;

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_rc_output::connection_established()
{
    /* Request on connection. */
    if (_communication != nullptr)
    {
        using namespace kfly_comm;
        _communication->send(codec::generate_command(commands::GetRCOutputSettings));

        if (!isHidden())
            _communication->subscribe(kfly_comm::commands::GetControlSignals, 100);
    }
}

void ui_rc_output::auto_upload_changed(bool checked)
{
    _auto_upload_checked = checked;
}

void ui_rc_output::upload_now()
{
    upload_settings();
}

void ui_rc_output::rc_output_settings(kfly_comm::datagrams::RCOutputSettings msg)
{
    qDebug() << "got rc output settings";

    for (auto i = 0; i < 8; i++)
       _channels[i]->set_channel_enabled(msg.channel_enabled[i]);

    _channels[0]->set_mode(msg.mode_bank1);
    _channels[4]->set_mode(msg.mode_bank2);
}

void ui_rc_output::control_signals(kfly_comm::datagrams::ControlSignals msg)
{
    qDebug() << "got control signals";

    for (auto i = 0; i < 8; i++)
        _channels[i]->set_bar_value(msg.motor_command[i]);
}

void ui_rc_output::upload_settings_timer()
{
    if (_upload_settings && _auto_upload_checked)
    {
        _upload_settings = false;
        upload_settings();
    }

    if (_override_enabled)
    {
        upload_override();
    }
}

void ui_rc_output::channel_value_changed()
{
    if (!isHidden())
    {
        qDebug() << "val change rc out";

        for (auto i = 1; i < 4; i++)
            _channels[i]->set_mode(_channels[0]->get_mode());

        for (auto i = 5; i < 8; i++)
            _channels[i]->set_mode(_channels[4]->get_mode());

        _upload_settings = true;
    }
}

void ui_rc_output::on_buttonOverrideSafeties_toggled(bool checked)
{
    if (checked)
    {
        auto reply = QMessageBox::question(
            this,
            "Propeller removal check",
            "WARNING! The propellers can spin! Make sure that " \
            "the propellers are removed! Have you done this?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            _override_enabled = true;

            for (auto i = 0; i < 8; i++)
                _channels[i]->set_override_enabled(true);

            ui->buttonStartStop->setEnabled(checked);
            ui->buttonNext->setEnabled(false);
        }
        else
        {
            _override_enabled = false;

            ui->buttonOverrideSafeties->setChecked(false);

            for (auto i = 0; i < 8; i++)
            {
                _channels[i]->set_override_enabled(false);
                _channels[i]->set_override_value(0);
            }
        }
    }
    else
    {
        _override_enabled = false;

        ui->buttonStartStop->setEnabled(false);
        ui->buttonNext->setEnabled(false);

        for (auto i = 0; i < 8; i++)
        {
            _channels[i]->set_override_enabled(false);
            _channels[i]->set_override_value(0);
        }
    }
}

void ui_rc_output::on_buttonStartStop_clicked()
{
    for (auto i = 0; i < 8; i++)
    {
        _channels[i]->set_override_value(100);
    }

    ui->buttonStartStop->setEnabled(false);
    ui->buttonNext->setEnabled(true);
}

void ui_rc_output::on_buttonNext_clicked()
{
    for (auto i = 0; i < 8; i++)
    {
        _channels[i]->set_override_value(0);
    }

    ui->buttonStartStop->setEnabled(true);
    ui->buttonNext->setEnabled(false);
}
