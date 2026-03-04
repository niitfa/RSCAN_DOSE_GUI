#ifndef WIDGETVOLTAGE_H
#define WIDGETVOLTAGE_H

#include <QWidget>
#include <QFont>

// add tcp client class

namespace Ui {
class WidgetVoltage;
}

class WidgetVoltage : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetVoltage(QWidget *parent = nullptr);
    ~WidgetVoltage();
    int getInputVoltage();
private slots:
    void on_pushButton_changeVoltage_clicked();

private:
    Ui::WidgetVoltage *ui;
    QFont buttonsFont;
};

#endif // WIDGETVOLTAGE_H
