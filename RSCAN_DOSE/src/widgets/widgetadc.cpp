#include "widgetadc.h"
#include "ui_widgetadc.h"

WidgetADC::WidgetADC(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetADC)
{
    ui->setupUi(this);
    setupFont();
    setupComboBoxOptions();
}

WidgetADC::~WidgetADC()
{
    delete ui;
}

void WidgetADC::setTCPClient(RSCANDoseClient *client)
{
    this->client = client;
}

void WidgetADC::setCommandCodes(RSCANDoseCommandCode setDR_code, RSCANDoseCommandCode setPGA_code)
{
    this->setDR_code = setDR_code;
    this->setPGA_code = setPGA_code;
}

void WidgetADC::setEnabled(bool en)
{
    QWidget::setEnabled(en);
    ui->label_DR->setEnabled(en);
    ui->label_PGA->setEnabled(en);
    ui->comboBox_DR->setEnabled(en);
    ui->comboBox_PGA->setEnabled(en);
    if(en)
    {
        ui->label_DR->setStyleSheet(
            "border-width:0px;" +
            this->enabledColorText
        );
        ui->label_PGA->setStyleSheet(
            "border-width:0px;" +
            this->enabledColorText
        );
        ui->comboBox_DR->setStyleSheet(
                    "border-width:0px;" +
                    this->enabledColorText
                );
        ui->comboBox_PGA->setStyleSheet(
                    "border-width:0px;" +
                    this->enabledColorText
                );
    }
    else
    {
        ui->label_DR->setStyleSheet(
            "border-width:0px;" +
            this->disabledColorText
        );
        ui->label_PGA->setStyleSheet(
            "border-width:0px;" +
            this->disabledColorText
        );
        ui->comboBox_DR->setStyleSheet(
                    "border-width:0px;" +
                    this->disabledColorText
                );
        ui->comboBox_PGA->setStyleSheet(
                    "border-width:0px;" +
                    this->disabledColorText
                );
    }
}

void WidgetADC::setDisabled(bool dis)
{
    WidgetADC::setEnabled(!dis);
}

void WidgetADC::setLabelText(QString labelPGA, QString labelDR)
{
    ui->label_PGA->setText(labelPGA);
    ui->label_DR->setText(labelDR);
}

void WidgetADC::setupComboBoxOptions()
{
    // PGA
    ui->comboBox_PGA->addItem("1");
    ui->comboBox_PGA->addItem("2");
    ui->comboBox_PGA->addItem("4");
    ui->comboBox_PGA->addItem("8");
    ui->comboBox_PGA->addItem("16");
    ui->comboBox_PGA->addItem("32");
    ui->comboBox_PGA->addItem("64");
    ui->comboBox_PGA->addItem("128");

    //DR
    ui->comboBox_DR->addItem("5");
    ui->comboBox_DR->addItem("10");
    ui->comboBox_DR->addItem("20");
    ui->comboBox_DR->addItem("40");
    ui->comboBox_DR->addItem("80");
    ui->comboBox_DR->addItem("160");
    ui->comboBox_DR->addItem("320");
    ui->comboBox_DR->addItem("640");
    ui->comboBox_DR->addItem("1000");
    ui->comboBox_DR->addItem("2000");
}

void WidgetADC::setupFont()
{
    QFont headFont;
    headFont.setFamily("Inter");
    headFont.setPixelSize(13);
    headFont.setWeight(60);
    ui->label_DR->setFont(headFont);
    ui->label_PGA->setFont(headFont);
    ui->comboBox_DR->setFont(headFont);
    ui->comboBox_PGA->setFont(headFont);

    this->enabledColorText =  "color: rgb(50,70,100);";
    this->disabledColorText = "color: rgb(130,130,130);";
}

#include <iostream>
void WidgetADC::on_comboBox_PGA_activated(const QString &arg1)
{
    if(client)
    {
        if(arg1 == "1") client->sendCommand(setPGA_code, 0b000);
        if(arg1 == "2") client->sendCommand(setPGA_code, 0b001);
        if(arg1 == "4") client->sendCommand(setPGA_code, 0b010);
        if(arg1 == "8") client->sendCommand(setPGA_code, 0b011);
        if(arg1 == "16") client->sendCommand(setPGA_code, 0b100);
        if(arg1 == "32") client->sendCommand(setPGA_code, 0b101);
        if(arg1 == "64") client->sendCommand(setPGA_code, 0b110);
        if(arg1 == "128") client->sendCommand(setPGA_code, 0b111);
    }
}

void WidgetADC::on_comboBox_DR_activated(const QString &arg1)
{
    if(client)
    {
        if(arg1 == "5") client->sendCommand(setDR_code, 0b0000);
        if(arg1 == "10") client->sendCommand(setDR_code, 0b0001);
        if(arg1 == "20") client->sendCommand(setDR_code, 0b0010);
        if(arg1 == "40") client->sendCommand(setDR_code, 0b0011);
        if(arg1 == "80") client->sendCommand(setDR_code, 0b0100);
        if(arg1 == "160") client->sendCommand(setDR_code, 0b0101);
        if(arg1 == "320") client->sendCommand(setDR_code, 0b0110);
        if(arg1 == "640") client->sendCommand(setDR_code, 0b0111);
        if(arg1 == "1000") client->sendCommand(setDR_code, 0b1000);
        if(arg1 == "2000") client->sendCommand(setDR_code, 0b1001);
    }
}
