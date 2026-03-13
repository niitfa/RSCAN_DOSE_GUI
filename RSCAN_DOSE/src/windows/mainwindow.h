#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "rscan_dose_client.h"
#include "settingswindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // connect widget callbacks
    void enableMainWindow();
    void disableMainWindow();

    // message received callback
    void updateWindowData();

    // signal widgets check box callbacks
    void showGraph(int graphNo, bool flag);
    void showGraphA(bool flag);
    void showGraphB(bool flag);

    // start logger callback
    void updateLoggerData();

private slots:
    void updateGraph();
    void on_pushButton_settings_clicked();
private:
    void setupClient();
    void setupChannelLabels();
    void setupSettingsButton();
    void setupConnectWidget();
    void setupVoltageWidgets();
    void setupSensitivityWidgets();
    void setupDisplayWidgets();
    void setupSignalWidgets();
    void setupLoggerWidget();
    void setupTimer();

private:
    Ui::MainWindow *ui;
    RSCANDoseClient* client = nullptr;
    SettingsWindow* settingsWindow = new SettingsWindow(this);

    // graph update management
    int id = 0;
    int graphInterval_ms = 5;
    QTimer* graphTimer = new QTimer();
};

#endif // MAINWINDOW_H
