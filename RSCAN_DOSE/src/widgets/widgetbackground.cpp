#include "widgetbackground.h"
#include "ui_widgetbackground.h"

WidgetBackground::WidgetBackground(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetBackground)
{
    ui->setupUi(this);
}

WidgetBackground::~WidgetBackground()
{
    delete ui;
}

void WidgetBackground::setClient(RSCANDoseClient *cli)
{
    client = cli;
}

void WidgetBackground::init()
{
    setMeasureTime(measureTime_s);
    setStatus(BackgroundMeasureStatus::Standby);
}

#include <iostream>
void WidgetBackground::update()
{
    measureUpdateTime();
    switch (status) {
    case BackgroundMeasureStatus::Standby:
        // standby
        break;
    case BackgroundMeasureStatus::Progress:
        progressTime_us += updatePeriodMicroseconds;
        fillProgress();
        if((progressTime_us / 1e+6) > measureTime_s)
        {
            setStatus(BackgroundMeasureStatus::Standby);
        }
        break;
    default:
        break;
    }
}

void WidgetBackground::on_lineEdit_time_editingFinished()
{
    bool ok = false;
    int newMeasureTime_s = ui->lineEdit_time->text().toInt(&ok);
    if(ok)
    {
        setMeasureTime(newMeasureTime_s);
    }
    setMeasureTime(measureTime_s);
}

void WidgetBackground::setMeasureTime(int time_s)
{
    (time_s > 0) ? measureTime_s = time_s : 0;
    ui->lineEdit_time->setText(QString::number(time_s));
    resetProgress();
}

void WidgetBackground::fillProgress()
{
    uint32_t progressTime_s = progressTime_us / 1e+6;
    (progressTime_s < 0) ? progressTime_s = 0 : 0;
    (progressTime_s > measureTime_s) ? progressTime_s = measureTime_s : 0;
    int progressPercent = (static_cast<double>(progressTime_s) / measureTime_s * 100);
    ui->progressBar_background->setFormat(QString::number(progressTime_s) + "/" + QString::number(measureTime_s));
    ui->progressBar_background->setValue(progressPercent);
}

void WidgetBackground::resetProgress()
{
    progressTime_us = 0;
    fillProgress();
}

void WidgetBackground::measureUpdateTime()
{
    auto endDuration = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = endDuration - this->startDuration;
    this->updatePeriodMicroseconds.store(static_cast<uint32_t>(elapsedSeconds.count() * 1e+6));
    this->startDuration = std::chrono::system_clock::now();
}

void WidgetBackground::setStatus(WidgetBackground::BackgroundMeasureStatus s)
{
    status = s;
    switch (status) {
    case BackgroundMeasureStatus::Standby:
        status = BackgroundMeasureStatus::Standby;
        setStandbyButtonStyle(ui->pushButton_background);
        ui->lineEdit_time->setEnabled(1);
        break;
    case BackgroundMeasureStatus::Progress:
        resetProgress();
        status = BackgroundMeasureStatus::Progress;
        setProgressButtonStyle(ui->pushButton_background);
        ui->lineEdit_time->setEnabled(0);
        break;
    default:
        break;
    }
}

void WidgetBackground::setStandbyButtonStyle(QPushButton *b)
{
    if(b)
    {
        b->setText("Измерить");
    }
}

void WidgetBackground::setProgressButtonStyle(QPushButton *b)
{
    if(b)
    {
        b->setText("Стоп");
    }
}

void WidgetBackground::on_pushButton_background_clicked()
{
    switch (status) {
    case BackgroundMeasureStatus::Standby:
        setStatus(BackgroundMeasureStatus::Progress);
        break;
    case BackgroundMeasureStatus::Progress:
        setStatus(BackgroundMeasureStatus::Standby);
        break;
    default:
        break;
    }
}
