#ifndef WIDGETBACKGROUND_H
#define WIDGETBACKGROUND_H

#include <QWidget>
#include <QPushButton>
#include "rscan_dose_client.h"
#include "weightedaverage.h"

namespace Ui {
class WidgetBackground;
}

class WidgetBackground : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetBackground(QWidget *parent = nullptr);
    ~WidgetBackground();
    void setClient(RSCANDoseClient*);
    void init();
    void setSensitivityValueCode(RSCANDoseValueCode code);
    void setDoseValueCode(RSCANDoseValueCode code);
    void update();
    void setCurrentCoeff(double low, double high);
    double getBackgroundChecked();
    double getBackground(RSCANDoseSensitivity);

    void setEnabled(bool);
    void setDisabled(bool);

private slots:
    void on_lineEdit_time_editingFinished();

    void on_pushButton_background_clicked();

private:
    Ui::WidgetBackground *ui;
    RSCANDoseClient* client = nullptr;
    uint32_t measureTime_s = 60;
    uint32_t progressTime_us = 0;
    WeightedAverage backgroundLow;
    WeightedAverage backgroundHigh;
    double kLow = 1;
    double kHigh = 1;
    std::atomic<uint32_t> updatePeriodMicroseconds { 0 };
    std::chrono::_V2::system_clock::time_point startDuration;
    RSCANDoseValueCode sensitivityCode = RSCANDoseValueCode::Sensitivity_A;
    RSCANDoseValueCode doseCode = RSCANDoseValueCode::Dose_A;

    enum class BackgroundMeasureStatus{
        Standby,
        Progress
    } ;

    BackgroundMeasureStatus status = BackgroundMeasureStatus::Standby;

private:
    void setStyle();
    void setMeasureTime(uint32_t time_s);
    void fillProgress();
    void resetProgress();
    void measureUpdateTime();
    void setStatus(BackgroundMeasureStatus s);
    void setStandbyButtonStyle(QPushButton*);
    void setProgressButtonStyle(QPushButton*);

    void resetBackground(RSCANDoseSensitivity);
    void printBackground(RSCANDoseSensitivity, double);
    void updateBackground();

    double getBackground();
    bool backgroundSubstractionChecked();

};

#endif // WIDGETBACKGROUND_H
