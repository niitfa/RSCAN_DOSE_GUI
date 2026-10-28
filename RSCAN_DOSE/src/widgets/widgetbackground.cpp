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
    setStyle();
    setMeasureTime(measureTime_s);
    setStatus(BackgroundMeasureStatus::Standby);
    resetBackground(RSCANDoseSensitivity::Low);
    resetBackground(RSCANDoseSensitivity::High);
}

void WidgetBackground::setSensitivityValueCode(RSCANDoseValueCode code)
{
    sensitivityCode = code;
}

void WidgetBackground::setDoseValueCode(RSCANDoseValueCode code)
{
    doseCode = code;
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
        updateBackground();
        if((progressTime_us / 1e+6) > measureTime_s)
        {
            setStatus(BackgroundMeasureStatus::Standby);
        }
        break;
    }
}

void WidgetBackground::setCurrentCoeff(double low, double high)
{
    kLow = low;
    kHigh = high;
}

double WidgetBackground::getBackgroundChecked()
{
    if(backgroundSubstractionChecked())
    {
        return getBackground();
    }
    return 0;
}

double WidgetBackground::getBackground(RSCANDoseSensitivity sensitivity)
{
    double bg = 0;
    switch (sensitivity)
    {
    case RSCANDoseSensitivity::Low:
        bg = backgroundLow.get();
        break;
    case RSCANDoseSensitivity::High:
        bg = backgroundHigh.get();
        break;
    }
    return bg;
}

void WidgetBackground::setEnabled(bool en)
{
    QWidget::setEnabled(en);

}

void WidgetBackground::setDisabled(bool dis)
{
    WidgetBackground::setEnabled(!dis);
}

double WidgetBackground::getBackground()
{
    double bg = 0;
    if(client)
    {
        RSCANDoseSensitivity sensitivity = static_cast<RSCANDoseSensitivity>(client->readValue(sensitivityCode));
        bg = getBackground(sensitivity);
    }
    return bg;
}

bool WidgetBackground::backgroundSubstractionChecked()
{
    return ui->checkBox_background->isChecked();
}

void WidgetBackground::on_lineEdit_time_editingFinished()
{
    bool ok = false;
    uint32_t newMeasureTime_s = ui->lineEdit_time->text().toUInt(&ok);
    if(ok)
    {
        setMeasureTime(newMeasureTime_s);
    }
    setMeasureTime(measureTime_s);
}

void WidgetBackground::setMeasureTime(uint32_t time_s)
{
    (time_s > 0) ? (measureTime_s = time_s) : 0;
    ui->lineEdit_time->setText(QString::number(time_s));
    resetProgress();
}

void WidgetBackground::fillProgress()
{
    uint32_t progressTime_s = progressTime_us / 1000000;
    (progressTime_s > measureTime_s) ? progressTime_s = measureTime_s : 0;
    int progressPercent = static_cast<int>((static_cast<double>(progressTime_s) / measureTime_s * 100));
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
        if(client)
        {
            resetBackground(static_cast<RSCANDoseSensitivity>(client->readValue(sensitivityCode)));
        }
        status = BackgroundMeasureStatus::Progress;
        setProgressButtonStyle(ui->pushButton_background);
        ui->lineEdit_time->setEnabled(0);
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

void WidgetBackground::resetBackground(RSCANDoseSensitivity sensitivity)
{
    switch (sensitivity)
    {
    case RSCANDoseSensitivity::Low:
        backgroundLow.reset();
        printBackground(sensitivity, backgroundLow.get());
        break;
    case RSCANDoseSensitivity::High:
        backgroundHigh.reset();
        printBackground(sensitivity, backgroundHigh.get());
        break;
    }
}

void WidgetBackground::printBackground(RSCANDoseSensitivity sensitivity, double bg)
{
    switch (sensitivity)
    {
    case RSCANDoseSensitivity::Low:
        ui->label_lowBackground->setText(QString::number(bg) + " пА");
        break;
    case RSCANDoseSensitivity::High:
        ui->label_highBackground->setText(QString::number(bg) + " пА");
        break;
    }
}

void WidgetBackground::updateBackground()
{
    if(client)
    {
        RSCANDoseSensitivity sensitivity = static_cast<RSCANDoseSensitivity>(client->readValue(sensitivityCode));
        double bg = 0;
        switch (sensitivity)
        {
        case RSCANDoseSensitivity::Low:
            backgroundLow.add(client->readValue(doseCode) * kLow, updatePeriodMicroseconds.load());
            bg = backgroundLow.get();
            break;
        case RSCANDoseSensitivity::High:
            backgroundHigh.add(client->readValue(doseCode) * kHigh, updatePeriodMicroseconds.load());
            bg = backgroundHigh.get();
            break;
        }
        printBackground(sensitivity, bg);
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
    }
}

void WidgetBackground::setStyle()
{
    /*QFont headFont;
    headFont.setFamily("Inter");
    headFont.setPixelSize(13);
    headFont.setWeight(60);
    ui->label_DR->setFont(headFont);
    ui->label_PGA->setFont(headFont);
    ui->comboBox_DR->setFont(headFont);
    ui->comboBox_PGA->setFont(headFont);

    this->enabledColorText =  "color: rgb(50,70,100);";
    this->disabledColorText = "color: rgb(130,130,130);"; */

}
