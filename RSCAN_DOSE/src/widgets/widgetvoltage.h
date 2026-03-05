#ifndef WIDGETVOLTAGE_H
#define WIDGETVOLTAGE_H

#include <QWidget>
#include <QFont>
#include <QPushButton>
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

private slots:
    void on_pushButton_changeVoltage_clicked();
    void on_pushButton_plus_clicked();
    void on_pushButton_minus_clicked();

private:
    void setEnabledStyle(QPushButton*);
    void setDisabledStyle(QPushButton*);
    void setInactiveStyle(QPushButton*);

    void setPlusButtonEnabled();
    void setPlusButtonDisabled();
    void setMinusButtonEnabled();
    void setMinusButtonDisabled();
    void setPolarity(uint8_t polarity);

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

private:
    Ui::WidgetVoltage *ui;
    QFont buttonsFont;
    uint8_t lastPolarity = 1;
    uint8_t lastEnabled = 0;
    RSCANDoseClient* client = nullptr;
};

#endif // WIDGETVOLTAGE_H
