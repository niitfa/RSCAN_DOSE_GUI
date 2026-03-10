#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void enableMainWindow();
    void disableMainWindow();
    void updateWindowData();

private slots:
    void on_pushButton_settings_clicked();
private:
    void setupClient();
    void setupSettingsButton();
    void setupConnectWidget();
    void setupVoltageWidgets();
    void setupSensitivityWidgets();
    void setupDisplayWidgets();

private:
    Ui::MainWindow *ui;
    RSCANDoseClient* client = nullptr;
    SettingsWindow* settingsWindow = new SettingsWindow(this);
};

#endif // MAINWINDOW_H
