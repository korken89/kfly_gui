#include "ui_developer.h"
#include "ui_ui_developer.h"

ui_developer::ui_developer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_developer)
{
    ui->setupUi(this);
}

ui_developer::~ui_developer()
{
    delete ui;
}

void ui_developer::register_communication(communication *com)
{
    _communication = com;

    connect(&_upload_settings_timer, &QTimer::timeout,
            this, &ui_developer::upload_settings_timer);

    _upload_settings_timer.start(1000);
}

void ui_developer::showEvent(QShowEvent *)
{
}

void ui_developer::hideEvent(QHideEvent *)
{
}

void ui_developer::upload_settings()
{
}

void ui_developer::connection_established()
{
}

void ui_developer::auto_upload_changed(bool checked)
{
    _auto_upload_checked = checked;
}

void ui_developer::upload_now()
{
    upload_settings();
}

void ui_developer::upload_settings_timer()
{
    if (_upload_settings && _auto_upload_checked)
    {
        _upload_settings = false;
        upload_settings();
    }
}
