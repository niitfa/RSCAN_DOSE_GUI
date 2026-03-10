#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>
#include <iostream>
#include "settings.h"

// команда на выключение высокого так же обнуляет выход ЦАП - на МК
// при закрытии приложения отправлять команду на отключение высокого?

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("RSCAN DOSE");
    setupClient();
    setupSettingsButton();
    setupConnectWidget();
    setupVoltageWidgets();
    setupSensitivityWidgets();
    setupDisplayWidgets();

    disableMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableMainWindow()
{
    // channel A
    ui->widget_sensitivityA->setEnabled(1);
    ui->widget_voltageA->setEnabled(1);
    ui->widget_voltageDisplayA->setEnabled(1);
    // channel B
    ui->widget_sensitivityB->setEnabled(1);
    ui->widget_voltageB->setEnabled(1);
    ui->widget_voltageDisplayB->setEnabled(1);
    // other
    ui->widget_temperatureDisplay->setEnabled(1);

}

void MainWindow::disableMainWindow()
{
    // channel A
    ui->widget_sensitivityA->setDisabled(1);
    ui->widget_voltageA->setDisabled(1);
    ui->widget_voltageDisplayA->setDisabled(1);
    // channel B
    ui->widget_sensitivityB->setDisabled(1);
    ui->widget_voltageB->setDisabled(1);
    ui->widget_voltageDisplayB->setDisabled(1);
    // other
    ui->widget_temperatureDisplay->setDisabled(1);
}

void MainWindow::updateWindowData()
{
    if(client)
    {
        // voltage A
        ui->widget_voltageDisplayA->setValueText(QString::number(client->readValue(RSCANDoseValueCode::HV_A)));
        // voltage B
        ui->widget_voltageDisplayB->setValueText(QString::number(client->readValue(RSCANDoseValueCode::HV_B)));
        // temperature
        ui->widget_temperatureDisplay->setValueText(QString::number(client->readValue(RSCANDoseValueCode::Temp_A)));
    }
}

void MainWindow::setupClient()
{
    std::string ip = Settings::get(Settings::Key::IP, Settings::Section::Main).toString().toStdString();
    uint16_t port = static_cast<uint16_t>(Settings::get(Settings::Key::Port, Settings::Section::Main).toInt());

    client = new RSCANDoseClient(ip, port);
    client->setIP(ip);
    client->setPort(port);
    client->setVerbose(true);
    std::function<void()> msgRecvCallback(std::bind(&MainWindow::updateWindowData, this));
    client->setMessageReceivedCallback(msgRecvCallback);
}

void MainWindow::setupSettingsButton()
{
    QPushButton* button = ui->pushButton_settings;
    button->setText("");
    button->setStyleSheet("* { background-color: rgb(220,220,220) }");

    /* Icon */
    double iconScaleFactor = 0.70;
    QSize size = button->size();
    size.setWidth(static_cast<int>(size.width() * iconScaleFactor));
    size.setHeight(static_cast<int>(size.height() * iconScaleFactor));
    button->setIcon(QIcon(":/img/button_icon_settings.png"));
    button->setIconSize(size);
}

void MainWindow::setupConnectWidget()
{
    ui->widget_connect->setTCPClient(this->client);
    std::function<void()> enableCallback(std::bind(&MainWindow::enableMainWindow, this));
    ui->widget_connect->setConnectionEstablishedCallback(enableCallback);
    std::function<void()> disableCallback(std::bind(&MainWindow::disableMainWindow, this));
    ui->widget_connect->setConnectionLostCallback(disableCallback);
}

void MainWindow::setupVoltageWidgets()
{
    // channel A configuration
    ui->widget_voltageA->setTCPClient(this->client);
    ui->widget_voltageA->setCommandCodes(
                RSCANDoseCommandCode::Enable_HV_A,
                RSCANDoseCommandCode::Disable_HV_A,
                RSCANDoseCommandCode::Set_Voltage_A,
                RSCANDoseCommandCode::Set_HV_Polarity_A
                );

    ui->widget_voltageA->setValueCodes(
                RSCANDoseValueCode::Dose_A,
                RSCANDoseValueCode::HV_A,
                RSCANDoseValueCode::Temp_A,
                RSCANDoseValueCode::Sensitivity_A,
                RSCANDoseValueCode::HV_Polarity_A,
                RSCANDoseValueCode::HV_Enabled_A
                );

    // channel B configuration
    ui->widget_voltageB->setTCPClient(this->client);
    ui->widget_voltageB->setCommandCodes(
                RSCANDoseCommandCode::Enable_HV_B,
                RSCANDoseCommandCode::Disable_HV_B,
                RSCANDoseCommandCode::Set_Voltage_B,
                RSCANDoseCommandCode::Set_HV_Polarity_B
                );

    ui->widget_voltageB->setValueCodes(
                RSCANDoseValueCode::Dose_B,
                RSCANDoseValueCode::HV_B,
                RSCANDoseValueCode::Temp_B,
                RSCANDoseValueCode::Sensitivity_B,
                RSCANDoseValueCode::HV_Polarity_B,
                RSCANDoseValueCode::HV_Enabled_B
                );

}

void MainWindow::setupSensitivityWidgets()
{
    // channel A
    ui->widget_sensitivityA->setTCPClient(this->client);
    ui->widget_sensitivityA->setCommandCodes(
                RSCANDoseCommandCode::Set_Sensitivity_A
                );
    ui->widget_sensitivityA->setValueCodes(
                RSCANDoseValueCode::Sensitivity_A
                );

    // channel B
    ui->widget_sensitivityB->setTCPClient(this->client);
    ui->widget_sensitivityB->setCommandCodes(
                RSCANDoseCommandCode::Set_Sensitivity_B
                );
    ui->widget_sensitivityB->setValueCodes(
                RSCANDoseValueCode::Sensitivity_B
                );
}

void MainWindow::setupDisplayWidgets()
{
    // channel A voltage display
    ui->widget_voltageDisplayA->setHeadText("Напряжение (А), В:");
    // channel B voltage display
    ui->widget_voltageDisplayB->setHeadText("Напряжение (B), В:");
    // temperature
    ui->widget_temperatureDisplay->setHeadText("Температура, ℃:");

}

void MainWindow::on_pushButton_settings_clicked()
{
    settingsWindow->show();
}
