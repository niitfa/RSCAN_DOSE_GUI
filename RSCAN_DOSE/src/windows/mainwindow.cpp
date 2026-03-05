#include "mainwindow.h"
#include "ui_mainwindow.h"
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
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_pushButton_settings_clicked()
{
    settingsWindow->show();
}
