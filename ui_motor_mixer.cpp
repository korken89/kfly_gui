#include "ui_motor_mixer.h"
#include "ui_ui_motor_mixer.h"

ui_motor_mixer::ui_motor_mixer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_motor_mixer)
{
    ui->setupUi(this);

    _auto_upload_checked = false;

    for (int i = 0; i < 8; i++)
    {
        ui_motor_mixer_channel *chan = new ui_motor_mixer_channel;
        _channels.push_back(chan);

        chan->set_channel_number(i+1);

        /* Connect each channel to know when any value has changed. */
        connect(chan, &ui_motor_mixer_channel::value_changed,
                this, &ui_motor_mixer::channel_value_changed);

        ui->layoutChannels->addWidget(chan);
    }
}

ui_motor_mixer::~ui_motor_mixer()
{
  delete ui;
}

void ui_motor_mixer::register_communication(communication *com)
{
    _communication = com;

    connect(_communication, &communication::sigChannelMix,
            this, &ui_motor_mixer::rc_mixer_settings);

    connect(&_upload_settings_timer, &QTimer::timeout,
            this, &ui_motor_mixer::upload_settings_timer);

    _upload_settings_timer.start(1000);
}

void ui_motor_mixer::showEvent(QShowEvent *)
{
}

void ui_motor_mixer::hideEvent(QHideEvent *)
{
}

void ui_motor_mixer::upload_settings_timer()
{
    if (_upload_settings)
    {
        _upload_settings = false;
        upload_settings();
    }
}

void ui_motor_mixer::rc_mixer_settings(kfly_comm::datagrams::ChannelMix msg)
{
    qDebug() << "got mixer settings";

    for (unsigned i = 0; i < _channels.size(); i++)
    {
        _channels[i]->set_throttle_weight(msg.weights[i][0]);
        _channels[i]->set_roll_weight(msg.weights[i][1]);
        _channels[i]->set_pitch_weight(msg.weights[i][2]);
        _channels[i]->set_yaw_weight(msg.weights[i][3]);
        _channels[i]->set_offset(msg.offset[i]);
    }
}

void ui_motor_mixer::upload_settings()
{
    qDebug() << "uploading channel mixing settings";

    kfly_comm::datagrams::ChannelMix msg;

    for (unsigned i = 0; i < _channels.size(); i++)
    {
        msg.weights[i][0] = _channels[i]->get_throttle_weight();
        msg.weights[i][1] = _channels[i]->get_roll_weight();
        msg.weights[i][2] = _channels[i]->get_pitch_weight();
        msg.weights[i][3] = _channels[i]->get_yaw_weight();
        msg.offset[i]     = _channels[i]->get_offset();
    }

    if (_communication != nullptr)
        _communication->send(kfly_comm::codec::generate_packet(msg));
}

void ui_motor_mixer::connection_established()
{
    /* Request on connection. */
    if (_communication != nullptr)
    {
        using namespace kfly_comm;
        _communication->send(codec::generate_command(commands::GetChannelMix));
    }
}

void ui_motor_mixer::auto_upload_changed(bool checked)
{
    _auto_upload_checked = checked;
}

void ui_motor_mixer::upload_now()
{
    upload_settings();
}

void ui_motor_mixer::channel_value_changed()
{
    if (!isHidden() && _auto_upload_checked)
    {
        qDebug() << "val change mix";

        _upload_settings = true;
    }
}
