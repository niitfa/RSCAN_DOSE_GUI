#ifndef WIDGETVOLTAGE_H
#define WIDGETVOLTAGE_H

#include <QWidget>
#include <QFont>
#include <QIcon>
#include <QPushButton>
#include <QTimer>
#include "rscan_dose_client.h"

namespace Ui {
class WidgetVoltage;
}

class WidgetVoltage : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetVoltage(QWidget *parent = nullptr);
    ~WidgetVoltage();
    int getInputVoltage(); // needed?
    void setTCPClient(RSCANDoseClient*);
    void setCommandCodes(
            RSCANDoseCommandCode hvEnableCode,
            RSCANDoseCommandCode hvDisableCode,
            RSCANDoseCommandCode setVoltageCode,
            RSCANDoseCommandCode setPolarityCode
            );

    void setValueCodes(
            RSCANDoseValueCode getDoseCode,
            RSCANDoseValueCode getHVCode,
            RSCANDoseValueCode getTemperatureCode,
            RSCANDoseValueCode getSensitivityCode,
            RSCANDoseValueCode getPolarityCode,
            RSCANDoseValueCode getHVEnabledCode
            );

    // override
    void setEnabled(bool);
    void setDisabled(bool);

private slots:
    void watchdogTick();
    void on_pushButton_changeVoltage_clicked();
    void on_pushButton_plus_clicked();
    void on_pushButton_minus_clicked();
    void on_switch_hvEnable_clicked();

private:
    void setInactiveStyle(QPushButton* button, const QIcon& icon, int iconSize);
    void setActiveStyle(QPushButton* button, const QIcon& icon, int iconSize);

    void setPlusButtonActive(bool);
    void setMinusButtonActive(bool);

    // constructor wrappers
    // frames
    void drawExternalFrame();
    void drawValueFrame();
    void drawPolarityFrame();

    // icons, texts, buttons
    void drawHeadIcons();
    void drawHeadTexts();

    // setup font
    void setupFont();
    void setupSwitch();
    void setupChangeVoltageButton();
    void setupLineEditVoltage();

    // timer
    void watchdogEnable();
    void watchdogDisable();

    // other
    void setActiveWidgetsEnabled(bool);
    void setupWidgetsHVEnabled(bool);
    void setupWidgetsPolarity(bool);

    void enablePolarityButtons(bool);

private:
    Ui::WidgetVoltage *ui;
    QFont buttonsFont;
    uint8_t lastPolarity = 1;
    uint8_t lastEnabled = 0;
    RSCANDoseClient* client = nullptr;

    // command codes
    RSCANDoseCommandCode hvEnableCode = RSCANDoseCommandCode::Empty;
    RSCANDoseCommandCode hvDisableCode = RSCANDoseCommandCode::Empty;
    RSCANDoseCommandCode setVoltageCode = RSCANDoseCommandCode::Empty;
    RSCANDoseCommandCode setPolarityCode = RSCANDoseCommandCode::Empty;

    // value codes
    RSCANDoseValueCode getDoseCode = RSCANDoseValueCode::Message_Num;
    RSCANDoseValueCode getHVCode = RSCANDoseValueCode::Message_Num;
    RSCANDoseValueCode getTemperatureCode = RSCANDoseValueCode::Message_Num;
    RSCANDoseValueCode getSensitivityCode = RSCANDoseValueCode::Message_Num;
    RSCANDoseValueCode getPolarityCode = RSCANDoseValueCode::Message_Num;
    RSCANDoseValueCode getHVEnabledCode = RSCANDoseValueCode::Message_Num;

    // previous states
    bool previousHVEnabledState = false;
    bool previousPolarity = false;

    QTimer* timer = nullptr;
    int wdInterval_ms = 200;

    // voltage limit
    static const int maxVoltage = 500;
    static const int polaritySwitchLimitVoltage = maxVoltage * 0.05;
};

#endif // WIDGETVOLTAGE_H
