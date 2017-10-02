#ifndef UI_DEVELOPER_H
#define UI_DEVELOPER_H

#include <QWidget>

namespace Ui {
class ui_developer;
}

class ui_developer : public QWidget
{
    Q_OBJECT

public:
    explicit ui_developer(QWidget *parent = 0);
    ~ui_developer();

private:
    Ui::ui_developer *ui;
};

#endif // UI_DEVELOPER_H
