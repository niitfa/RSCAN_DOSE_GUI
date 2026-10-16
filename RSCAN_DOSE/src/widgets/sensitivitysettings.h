#ifndef SENSITIVITYSETTINGS_H
#define SENSITIVITYSETTINGS_H

#include <QWidget>

namespace Ui {
class SensitivitySettings;
}

class SensitivitySettings : public QWidget
{
    Q_OBJECT

public:
    explicit SensitivitySettings(QWidget *parent = nullptr);
    ~SensitivitySettings();
    void applySettings();
    void show();

private slots:
    void on_lineEdit_k_A_low_editingFinished();

    void on_lineEdit_k_A_high_editingFinished();

    void on_lineEdit_k_B_low_editingFinished();

    void on_lineEdit_k_B_high_editingFinished();

private:
    Ui::SensitivitySettings *ui;

private:
    void underlineTitle();
    void showAll();
};

#endif // SENSITIVITYSETTINGS_H
