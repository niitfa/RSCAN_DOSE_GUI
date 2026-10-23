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
    setupChannelLabels();
    setupClient();
    setupSettingsButton();
    setupConnectWidget();
    setupVoltageWidgets();
    setupSensitivityWidgets();
    setupDisplayWidgets();
    setupSignalWidgets();
    setupLoggerWidget();
    setupADCWidgets();
    setupTimer();
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
    ui->widget_signalA->setEnabled(1);
    ui->widget_adcA->setEnabled(1);
    // channel B
    ui->widget_sensitivityB->setEnabled(1);
    ui->widget_voltageB->setEnabled(1);
    ui->widget_voltageDisplayB->setEnabled(1);
    ui->widget_signalB->setEnabled(1);
    ui->widget_adcB->setEnabled(1);
    // labels
    //ui->widget_headA->setEnabled(1);
    //ui->widget_headB->setEnabled(1);
    // other
    ui->widget_temperatureDisplay->setEnabled(1);
    ui->widget_file->setEnabled(1);
    ui->widget_graph->setEnabled(1);
    // timer
    graphTimer->start();
}

void MainWindow::disableMainWindow()
{
    // channel A
    ui->widget_sensitivityA->setDisabled(1);
    ui->widget_voltageA->setDisabled(1);
    ui->widget_voltageDisplayA->setDisabled(1);
    ui->widget_signalA->setDisabled(1);
    ui->widget_adcA->setDisabled(1);
    // channel B
    ui->widget_sensitivityB->setDisabled(1);
    ui->widget_voltageB->setDisabled(1);
    ui->widget_voltageDisplayB->setDisabled(1);
    ui->widget_signalB->setDisabled(1);
    ui->widget_adcB->setDisabled(1);
    // labels
    //ui->widget_headA->setDisabled(1);
    //ui->widget_headB->setDisabled(1);
    // other
    ui->widget_temperatureDisplay->setDisabled(1);
    ui->widget_file->setDisabled(1);
    ui->widget_graph->setDisabled(1);
    // timer
    graphTimer->stop();
}

void MainWindow::updateWindowData()
{
    if(client)
    {
        // signal A
        //ui->widget_signalA->setValueText(QString::number(client->readValue(RSCANDoseValueCode::Dose_A) * kA()) );

        if(averageSignalA) { ui->widget_signalA->setValueText(QString::number(averageSignalA->getAverage())); }
        // signal B
        //ui->widget_signalB->setValueText(QString::number(client->readValue(RSCANDoseValueCode::Dose_B) * kB()));
        if(averageSignalB) { ui->widget_signalB->setValueText(QString::number(averageSignalB->getAverage())); }
        // voltage A
        ui->widget_voltageDisplayA->setValueText(QString::number(client->readValue(RSCANDoseValueCode::HV_A)));
        // voltage B
        ui->widget_voltageDisplayB->setValueText(QString::number(client->readValue(RSCANDoseValueCode::HV_B)));
        // temperature
        ui->widget_temperatureDisplay->setValueText(QString::number(client->readValue(RSCANDoseValueCode::Temp_A)));

        if(ui->widget_file->getLogger())
        {
            updateLoggerData();
            ui->widget_file->update(client->readValue(RSCANDoseValueCode::Message_Num));
        }

    }
}

void MainWindow::showGraph(int graphNo, bool flag)
{
    ui->widget_graph->setLineVisible(graphNo, flag);
}

void MainWindow::showGraphA(bool flag)
{
     showGraph(0, flag);
}

void MainWindow::showGraphB(bool flag)
{
    showGraph(1, flag);
}

void MainWindow::updateWindow()
{
    int msgNum = client->readValue(RSCANDoseValueCode::Message_Num);
    if(msgNum != id)
    {
        id = msgNum;
        double dataA = 0;
        double dataB = 0;
        if(averageSignalA)
        {
           averageSignalA->addElement( static_cast<double>( client->readValue(RSCANDoseValueCode::Dose_A) * kA() ));
           dataA = averageSignalA->getAverage();
        }
        if(averageSignalB)
        {
           averageSignalB->addElement( static_cast<double>( client->readValue(RSCANDoseValueCode::Dose_B) * kB() ));
           dataB = averageSignalB->getAverage();
        }
        QVector<double> data{
            dataA,
            dataB
         };
        ui->widget_graph->pushData(data);
        updateWindowData();
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
    ui->widget_voltageDisplayA->setHeadText("Напряжение (1), В:");
    // channel B voltage display
    ui->widget_voltageDisplayB->setHeadText("Напряжение (2), В:");
    // temperature
    ui->widget_temperatureDisplay->setHeadText("Темп., ℃:");
    ui->widget_temperatureDisplay->hide();
}

void MainWindow::setupSignalWidgets()
{
    std::function<void(bool)> showGraphACallback(std::bind(&MainWindow::showGraphA, this, std::placeholders::_1));
    std::function<void(bool)> showGraphBCallback(std::bind(&MainWindow::showGraphB, this, std::placeholders::_1));

    ui->widget_signalA->setHeadText("Ток (1), пА:");
    ui->widget_signalA->setCheckedCallback(showGraphACallback);
    ui->widget_signalA->setChecked(true);
    ui->widget_signalA->setMovingAverage(averageSignalA);
    ui->widget_signalB->setHeadText("Ток (2), пА:");
    ui->widget_signalB->setCheckedCallback(showGraphBCallback);
    ui->widget_signalB->setChecked(true);
    ui->widget_signalB->setMovingAverage(averageSignalB);
}

void MainWindow::setupLoggerWidget()
{
    ui->widget_file->setLogger(new Logger);

    std::function<void(void)> startLoggingCallback (std::bind(&MainWindow::updateLoggerData, this));
    ui->widget_file->setStartLoggingCallback(startLoggingCallback);
}

void MainWindow::setupADCWidgets()
{
    ui->widget_adcA->setLabelText("Усиление (1):", "Частота (1):");
    ui->widget_adcA->setTCPClient(client);
    ui->widget_adcA->setCommandCodes(
                RSCANDoseCommandCode::Set_ADC_DR_A, RSCANDoseCommandCode::Set_ADC_Gain_A
                );

    ui->widget_adcB->setLabelText("Усиление (2):", "Частота (2):");
    ui->widget_adcB->setTCPClient(client);
    ui->widget_adcB->setCommandCodes(
                RSCANDoseCommandCode::Set_ADC_DR_B, RSCANDoseCommandCode::Set_ADC_Gain_B
                );
}

void MainWindow::setupTimer()
{
    graphTimer->setSingleShot(false);
    graphTimer->setInterval(this->graphInterval_ms);
    QObject::connect(graphTimer, SIGNAL(timeout()), this, SLOT(updateWindow()));
}

double MainWindow::kA()
{
    double res = 1;
    int sensitivity = client->readValue(RSCANDoseValueCode::Sensitivity_A);
    if(sensitivity == static_cast<int>(RSCANDoseSensitivity::Low))
    {
        res = Settings::get(Settings::Key::K_A_Low, Settings::Section::Main).toDouble();
    }
    else if (sensitivity == static_cast<int>(RSCANDoseSensitivity::High))
    {
        res = Settings::get(Settings::Key::K_A_High, Settings::Section::Main).toDouble();
    }
    return res * 1e+12;
}

double MainWindow::kB()
{
    double res = 1;
    int sensitivity = client->readValue(RSCANDoseValueCode::Sensitivity_B);
    if(sensitivity == static_cast<int>(RSCANDoseSensitivity::Low))
    {
        res = Settings::get(Settings::Key::K_B_Low, Settings::Section::Main).toDouble();
    }
    else if (sensitivity == static_cast<int>(RSCANDoseSensitivity::High))
    {
        res = Settings::get(Settings::Key::K_B_High, Settings::Section::Main).toDouble();
    }
    return res * 1e+12;
}

void MainWindow::updateLoggerData()
{
    Logger* logger = ui->widget_file->getLogger();
    if(logger && client)
    {
        logger->setSignals(
                    client->readValue(RSCANDoseValueCode::Dose_A),
                    client->readValue(RSCANDoseValueCode::Dose_B)
                    );

        logger->setCurrents(
                    client->readValue(RSCANDoseValueCode::Dose_A) * kA(),
                    client->readValue(RSCANDoseValueCode::Dose_B) * kB()
                    );
        logger->setVoltages(
                    client->readValue(RSCANDoseValueCode::HV_A),
                    client->readValue(RSCANDoseValueCode::HV_B)
                    );
        logger->setPolarities(
                    client->readValue(RSCANDoseValueCode::HV_Polarity_A),
                    client->readValue(RSCANDoseValueCode::HV_Polarity_B)
                    );

        logger->setSensitivities(
                    client->readValue(RSCANDoseValueCode::Sensitivity_A),
                    client->readValue(RSCANDoseValueCode::Sensitivity_B)
                    );
        logger->setHVEnabled(
                    client->readValue(RSCANDoseValueCode::HV_Enabled_A),
                    client->readValue(RSCANDoseValueCode::HV_Enabled_B)
                    );
        logger->setHVEnabled(
                    client->readValue(RSCANDoseValueCode::HV_Enabled_A),
                    client->readValue(RSCANDoseValueCode::HV_Enabled_B)
                    );
        logger->setTemperature(
                    client->readValue(RSCANDoseValueCode::Temp_A)
                    );
    }
}

void MainWindow::setupChannelLabels()
{
}

void MainWindow::on_pushButton_settings_clicked()
{
    settingsWindow->show();
}
