#ifndef UI_DEVELOPER_H
#define UI_DEVELOPER_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include "communication.h"

namespace Ui {
class ui_developer;
}

class ui_developer : public QWidget
{
    Q_OBJECT

public:
    explicit ui_developer(QWidget *parent = 0);
    ~ui_developer();
    void register_communication(communication *com);
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;

private:
    Ui::ui_developer *ui;

    communication *_communication;
    bool _auto_upload_checked;
    bool _upload_settings;
    QTimer _upload_settings_timer;

    void upload_settings();

public slots:
    void connection_established();

    void auto_upload_changed(bool checked);
    void upload_now();

private slots:
    void upload_settings_timer();

    void on_buttonEraseAllSettings_clicked();
};

#endif // UI_DEVELOPER_H
