#include "ui_rc_output.h"
#include "ui_ui_rc_output.h"

ui_rc_output::ui_rc_output(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_rc_output)
{
    ui->setupUi(this);

    for (int i = 0; i < 8; i++)
    {
        ui_rc_output_channel *chan = new ui_rc_output_channel;
        _channels.push_back(chan);

        chan->set_channel_number(i+1);

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

void ui_rc_output::channel_value_changed()
{
    qDebug() << "val change rc out";
}

void ui_rc_output::on_buttonAutoUpload_toggled(bool checked)
{
    ui->buttonApplyChanges->setEnabled(!checked);
}
