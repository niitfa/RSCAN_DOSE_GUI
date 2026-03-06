#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>
#include <iostream>
#include "settings.h"

// команда на выключение высокого так же обнуляет выход ЦАП - на МК
// при закрытии приложения отправлять команду на отключение высокого?

// todo
// 1) виджет-коннект. Кнопка с иконкой, led статуса подключения и мб описание статуса (как в матрице)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("RSCAN DOSE");
    setupClient();
    setupSettingsButton();
    setupConnectWidget();

    disableMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableMainWindow()
{
    ui->widget_sensitivityA->setEnabled(1);
    ui->widget_voltageA->setEnabled(1);
}

void MainWindow::disableMainWindow()
{
    ui->widget_sensitivityA->setDisabled(1);
    ui->widget_voltageA->setDisabled(1);
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

void MainWindow::on_pushButton_settings_clicked()
{
    settingsWindow->show();
}
