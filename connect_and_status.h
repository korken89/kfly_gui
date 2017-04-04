#ifndef CONNECT_AND_STATUS_H
#define CONNECT_AND_STATUS_H

#include <QWidget>

namespace Ui {
class connect_and_status;
}

class connect_and_status : public QWidget
{
    Q_OBJECT

public:
    explicit connect_and_status(QWidget *parent = 0);
    ~connect_and_status();

private:
    Ui::connect_and_status *ui;
};

#endif // CONNECT_AND_STATUS_H
