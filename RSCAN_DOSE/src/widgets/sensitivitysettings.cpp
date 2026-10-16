#include "sensitivitysettings.h"
#include "ui_sensitivitysettings.h"
#include "ui_networksettings.h"
#include <QFont>
#include <QSize>
#include <iostream>
#include "settings.h"

SensitivitySettings::SensitivitySettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensitivitySettings)
{
    ui->setupUi(this);
    underlineTitle();
    showAll();
}

SensitivitySettings::~SensitivitySettings()
{
    delete ui;
}

void SensitivitySettings::applySettings()
{
    Settings::set(Settings::Key::K_A_Low, Settings::Section::Main) = ui->lineEdit_k_A_low->text();
    Settings::set(Settings::Key::K_A_High, Settings::Section::Main) = ui->lineEdit_k_A_high->text();
    Settings::set(Settings::Key::K_B_Low, Settings::Section::Main) = ui->lineEdit_k_B_low->text();
    Settings::set(Settings::Key::K_B_High, Settings::Section::Main) = ui->lineEdit_k_B_high->text();
}

void SensitivitySettings::show()
{
    QWidget::show();
    showAll();
}

void SensitivitySettings::on_lineEdit_k_A_low_editingFinished()
{
    bool ok = false;
    QString typed = ui->lineEdit_k_A_low->text();
    typed.toDouble(&ok);
    if(!ok)
    {
        QString old = Settings::get(Settings::Key::K_A_Low, Settings::Section::Main).toString();
        ui->lineEdit_k_A_low->setText(old);
    }
}

void SensitivitySettings::on_lineEdit_k_A_high_editingFinished()
{
    bool ok = false;
    QString typed = ui->lineEdit_k_A_high->text();
    typed.toDouble(&ok);
    if(!ok)
    {
        QString old = Settings::get(Settings::Key::K_A_High, Settings::Section::Main).toString();
        ui->lineEdit_k_A_high->setText(old);
    }
}

void SensitivitySettings::on_lineEdit_k_B_low_editingFinished()
{
    bool ok = false;
    QString typed = ui->lineEdit_k_B_low->text();
    typed.toDouble(&ok);
    if(!ok)
    {
        QString old = Settings::get(Settings::Key::K_B_Low, Settings::Section::Main).toString();
        ui->lineEdit_k_B_low->setText(old);
    }
}

void SensitivitySettings::on_lineEdit_k_B_high_editingFinished()
{
    bool ok = false;
    QString typed = ui->lineEdit_k_B_high->text();
    typed.toDouble(&ok);
    if(!ok)
    {
        QString old = Settings::get(Settings::Key::K_B_High, Settings::Section::Main).toString();
        ui->lineEdit_k_B_high->setText(old);
    }
}

void SensitivitySettings::underlineTitle()
{
    ui->label_title->font();
    QFont font = ui->label_title->font();
    font.setUnderline(true);
    font.setWeight(QFont::Weight::Normal);
    ui->label_title->setFont(font);
}

void SensitivitySettings::showAll()
{
    ui->lineEdit_k_A_low->setText(
                Settings::get(Settings::Key::K_A_Low, Settings::Section::Main).toString()
                );
    ui->lineEdit_k_B_low->setText(
                Settings::get(Settings::Key::K_B_Low, Settings::Section::Main).toString()
                );
    ui->lineEdit_k_A_high->setText(
                Settings::get(Settings::Key::K_A_High, Settings::Section::Main).toString()
                );
    ui->lineEdit_k_B_high->setText(
                Settings::get(Settings::Key::K_B_High, Settings::Section::Main).toString()
                );

}
