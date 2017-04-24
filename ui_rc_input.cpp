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
    ui->boxInputType->addItem("CPPM");
    ui->boxInputType->addItem("PWM");
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
}

void ui_rc_input::rc_values(kfly_comm::datagrams::RCValues msg)
{
    (void)msg;
    qDebug() << "got rc values";
}

void ui_rc_input::rc_input_settings(kfly_comm::datagrams::RCInputSettings msg)
{
    (void)msg;
    qDebug() << "got rc input settings";
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
    _upload_settings = true;
}

void ui_rc_input::on_buttonAutoUpload_toggled(bool checked)
{
    ui->buttonApplyChanges->setEnabled(!checked);
}

void ui_rc_input::on_boxInputType_editTextChanged(const QString &)
{
    _upload_settings = true;
}

void ui_rc_input::on_buttonApplyChanges_clicked()
{
    upload_settings();
}

void ui_rc_input::on_boxInputType_currentTextChanged(const QString &)
{
    _upload_settings = true;
}
