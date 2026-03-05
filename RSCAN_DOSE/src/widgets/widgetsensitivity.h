#ifndef WIDGETSENSITIVITY_H
#define WIDGETSENSITIVITY_H

#include <QWidget>
#include <QFont>
#include <QPushButton>
#include "rscan_dose_client.h"

namespace Ui {
class WidgetSensitivity;
}

class WidgetSensitivity : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetSensitivity(QWidget *parent = nullptr);
    ~WidgetSensitivity();
    void setSensitivity(uint8_t);
    void setTCPClient(RSCANDoseClient*);
private slots:
    void on_pushButton_sensitivityLow_clicked();
    void on_pushButton_sensitivityHigh_clicked();

private:
    void setEnabledStyle(QPushButton*);
    void setDisabledStyle(QPushButton*);

    void setLowButtonEnabled();
    void setLowButtonDisabled();
    void setHighButtonEnabled();
    void setHighButtonDisabled();

    // setup ui
    void drawInternalFrame();
    void drawExternalFrame();
    void drawIcon();
    void drawHeadText();
    void setupButtonsStyle();

private:
    Ui::WidgetSensitivity *ui;
    QFont buttonsFont;
    uint8_t lastSensitivity = 1; // 0 - low, 1 - high
    RSCANDoseClient* client = nullptr;
};

#endif // WIDGETSENSIVITY_H
