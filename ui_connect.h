#ifndef UI_CONNECT_H
#define UI_CONNECT_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class ui_connect;
}

class ui_connect : public QWidget
{
    Q_OBJECT

public:
    explicit ui_connect(QWidget *parent = 0);
    ~ui_connect();

private:
    Ui::ui_connect *ui;
};

#endif // UI_CONNECT_H
