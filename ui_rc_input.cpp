#include "ui_rc_input.h"
#include "ui_ui_rc_input.h"

ui_rc_input::ui_rc_input(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_rc_input)
{
    ui->setupUi(this);


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
}

ui_rc_input::~ui_rc_input()
{
    delete ui;
}

void ui_rc_input::channel_value_changed()
{
    qDebug() << "val change";
}

void ui_rc_input::on_buttonAutoUpload_toggled(bool checked)
{
    ui->buttonApplyChanges->setEnabled(!checked);
}
