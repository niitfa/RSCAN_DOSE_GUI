#ifndef WIDGETSENSITIVITY_H
#define WIDGETSENSITIVITY_H

#include <QWidget>
#include <QFont>
#include <QPushButton>
#include <QTimer>
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
    void setTCPClient(RSCANDoseClient*);
    void setCommandCodes(RSCANDoseCommandCode setSensitivityCode);
    void setValueCodes(RSCANDoseValueCode getSensitivityCode);

    // override
    void setEnabled(bool);
    void setDisabled(bool);
private slots:
    void watchdogTick();
    void on_pushButton_sensitivityLow_clicked();
    void on_pushButton_sensitivityHigh_clicked();

private:
    void setInactiveStyle(QPushButton* button);
    void setActiveStyle(QPushButton* button);

    void setLowButtonActive(bool);
    void setHighButtonActive(bool);

    // setup ui
    void drawInternalFrame();
    void drawExternalFrame();
    void drawIcon();
    void drawHeadText();
    void setupButtonsStyle();

    // timer
    void watchdogEnable();
    void watchdogDisable();

    // other
    void setupSensitivityButtons(bool);
private:
    Ui::WidgetSensitivity *ui;
    QFont buttonsFont;
    uint8_t lastSensitivity = 1; // 0 - low, 1 - high
    RSCANDoseClient* client = nullptr;

    // command code
    RSCANDoseCommandCode setSensitivityCode = RSCANDoseCommandCode::Empty;
    // value code
    RSCANDoseValueCode getSensitivityCode = RSCANDoseValueCode::Message_Num;

    bool previousSensitivity = false;
    QTimer* timer = nullptr;
    int wdInterval_ms = 500;

};

#endif // WIDGETSENSIVITY_H
