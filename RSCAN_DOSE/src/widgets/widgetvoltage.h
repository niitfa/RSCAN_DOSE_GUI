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

    // override
    void setEnabled(bool);
    void setDisabled(bool);
private slots:
    void watchdogTick();
    void on_pushButton_changeVoltage_clicked();
    void on_pushButton_plus_clicked();
    void on_pushButton_minus_clicked();

private:
    void setEnabledStyle(QPushButton* button, const QIcon& icon, int iconSize);
    void setDisabledStyle(QPushButton* button, const QIcon& icon, int iconSize);
    void setBlockedStyle(QPushButton* button, const QIcon& icon, int iconSize);


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

private:
    Ui::WidgetVoltage *ui;
    QFont buttonsFont;
    uint8_t lastPolarity = 1;
    uint8_t lastEnabled = 0;
    RSCANDoseClient* client = nullptr;

    QTimer* timer = nullptr;
    int wdInterval_ms = 100;
};

#endif // WIDGETVOLTAGE_H
