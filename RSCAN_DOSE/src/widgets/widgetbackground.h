#ifndef WIDGETBACKGROUND_H
#define WIDGETBACKGROUND_H

#include <QWidget>
#include <QPushButton>
#include "rscan_dose_client.h"

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
    void update();

private slots:
    void on_lineEdit_time_editingFinished();

    void on_pushButton_background_clicked();

private:
    Ui::WidgetBackground *ui;
    RSCANDoseClient* client = nullptr;
    int measureTime_s = 60;
    uint32_t progressTime_us = 0;
    std::atomic<uint32_t> updatePeriodMicroseconds { 0 };
    std::chrono::_V2::system_clock::time_point startDuration;

    enum class BackgroundMeasureStatus{
        Standby,
        Progress
    } ;

    BackgroundMeasureStatus status = BackgroundMeasureStatus::Standby;

private:
    void setMeasureTime(int time_s);
    void fillProgress();
    void resetProgress();
    void measureUpdateTime();
    void setStatus(BackgroundMeasureStatus s);

    void setStandbyButtonStyle(QPushButton*);
    void setProgressButtonStyle(QPushButton*);


};

#endif // WIDGETBACKGROUND_H
