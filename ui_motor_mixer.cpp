#include "ui_motor_mixer.h"
#include "ui_ui_motor_mixer.h"

ui_motor_mixer::ui_motor_mixer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_motor_mixer)
{
    ui->setupUi(this);

    for (int i = 0; i < 8; i++)
    {
        ui_motor_mixer_channel *chan = new ui_motor_mixer_channel;
        _channels.push_back(chan);

        //chan->set_channel_number(i+1);

        /* Connect each channel to know when any value has changed. */
        //connect(chan, &ui_rc_output_channel::value_changed,
        //        this, &ui_rc_output::channel_value_changed);

        ui->layoutChannels->addWidget(chan);
    }
}

ui_motor_mixer::~ui_motor_mixer()
{
  delete ui;
}
