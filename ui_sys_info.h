#ifndef UI_SYS_INFO_H
#define UI_SYS_INFO_H

#include <QWidget>
#include "communication.h"

namespace Ui {
class ui_sys_info;
}

class ui_sys_info : public QWidget
{
    Q_OBJECT

public:
    explicit ui_sys_info(QWidget *parent = 0);
    ~ui_sys_info();
    void register_communication(communication *com);
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;


private:
    Ui::ui_sys_info *ui;
    communication *_communication;
    bool _auto_upload_checked;
    bool _upload_settings;
    QTimer _upload_settings_timer;

    void upload_settings();

public slots:
    void connection_established();

    void auto_upload_changed(bool checked);
    void upload_now();

    void system_strings(kfly_comm::datagrams::SystemStrings msg);
    void system_status(kfly_comm::datagrams::SystemStatus msg);

private slots:
    void upload_settings_timer();
    void on_editVehicleType_textChanged(const QString &);
    void on_editVehicleName_textChanged(const QString &);
};

#endif // UI_SYS_INFO_H
