#include "widgetvoltage.h"
#include "ui_widgetvoltage.h"
#include <QMessageBox>

WidgetVoltage::WidgetVoltage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetVoltage)
{
    ui->setupUi(this);

    // ui setup
    drawValueFrame();
    drawPolarityFrame();
    drawExternalFrame();
    drawHeadIcons();
    drawHeadTexts();
    setupFont();
    setupSwitch();
    setupChangeVoltageButton();
    setupLineEditVoltage();

    // logic setup
    //setPolarity(!this->lastPolarity);
}

WidgetVoltage::~WidgetVoltage()
{
    delete timer;
    delete ui;
}

void WidgetVoltage::drawExternalFrame()
{
    ui->frame_borderExternal->setStyleSheet(
                " QFrame { background-color: rgb(230,230,230); } "
                " QFrame { border-radius:15px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );

    ui->verticalLayout_borderExternal->setParent(nullptr);
    ui->frame_borderExternal->setLayout(ui->verticalLayout_borderExternal);
}

void WidgetVoltage::drawValueFrame()
{
    ui->frame_borderValue->setStyleSheet(
                " QFrame { background-color: rgb(247,247,247); } "
                " QFrame { border-radius:10px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );

    ui->horizontalLayout_borderValue->setParent(nullptr);
    ui->frame_borderValue->setLayout(ui->horizontalLayout_borderValue);
}

void WidgetVoltage::drawPolarityFrame()
{
    ui->frame_borderPolarity->setStyleSheet(
                " QFrame { background-color: rgb(247,247,247); } "
                " QFrame { border-radius:10px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );

    ui->horizontalLayout_borderPolarity->setParent(nullptr);
    ui->frame_borderPolarity->setLayout(ui->horizontalLayout_borderPolarity);

}

void WidgetVoltage::drawHeadIcons()
{
    int headIconSize = 24;
    QIcon headIcon(":/img/icon_voltage.png");
    ui->label_headIcon->clear();
    ui->label_headIcon->setPixmap(headIcon.pixmap(headIconSize,headIconSize));
    ui->label_headIcon->setStyleSheet(
                "border-width:0px;"
                );

    int polarityIconSize = 20;
    QIcon polarityIcon(":/img/icon_polarity.png");
    ui->label_polarityIcon->clear();
    ui->label_polarityIcon->setPixmap(polarityIcon.pixmap(polarityIconSize,polarityIconSize));
    ui->label_polarityIcon->setStyleSheet(
                "border-width:0px;"
                );

}

void WidgetVoltage::drawHeadTexts()
{
    QFont headFont;
    headFont.setFamily("Inter");
    headFont.setPixelSize(16);
    headFont.setWeight(50);

    QString headTextColor = "color: rgb(70,70,70);";

    ui->label_headText->clear();
    ui->label_headText->setStyleSheet(
                "border-width:0px;" +
                headTextColor
                );
    ui->label_headText->setFont(headFont);
    ui->label_headText->setText("Напряжение");

    ui->label_polarityText->clear();
    ui->label_polarityText->setStyleSheet(
                "border-width:0px;" +
                headTextColor
                );
    ui->label_polarityText->setFont(headFont);
    ui->label_polarityText->setText("Полярность");
}

void WidgetVoltage::setupFont()
{
    this->buttonsFont.setFamily("Inter");
    this->buttonsFont.setPixelSize(15);
    this->buttonsFont.setWeight(50);
}

void WidgetVoltage::setupSwitch()
{
    ui->switch_hvEnable->setFont(this->buttonsFont);
    ui->switch_hvEnable->setText("");
}

void WidgetVoltage::setupChangeVoltageButton()
{
    ui->pushButton_changeVoltage->setFont(buttonsFont);
    ui->pushButton_changeVoltage->setText("Изменить");
    ui->pushButton_changeVoltage->setFocusPolicy( Qt::FocusPolicy::NoFocus );
    ui->pushButton_changeVoltage->setStyleSheet(
                // unpressed
                "QPushButton { border-style: outset; }"
                "QPushButton { border-radius:5px; }"
                "QPushButton { border-width:1px; }"
                "QPushButton { border-color: rgb(50,100,210); }"
                "QPushButton { background-color: rgb(60,120,230); }"
                "QPushButton { color: white; }"
                // hover
                "QPushButton:hover { background-color: rgb(40,100,200);  }"
                // pressed
                "QPushButton:pressed { background-color: rgb(30,80,170);  }"
                // disabled
                "QPushButton:disabled { border-width:0px;  }"
                "QPushButton:disabled { background-color: rgb(200,200,200);  }"
                "QPushButton:disabled { color: rgb(100,100,100);  }"
                );
}

void WidgetVoltage::setupLineEditVoltage()
{
    // label Voltage value measurements
    QString textColor = "color: rgb(30,30,30);";
    ui->label_value->setStyleSheet(
                "border-width:0px;" +
                textColor
                );
    ui->label_value->setFont(buttonsFont);
    ui->label_value->setText("В");

    // line edit voltage input
    ui->lineEdit_voltageInput->setStyleSheet(
                "border-width:0px;" +
                textColor
                );
    ui->lineEdit_voltageInput->setFont(buttonsFont);
    ui->lineEdit_voltageInput->setText(QString::number(0));
}

void WidgetVoltage::watchdogEnable()
{
    if(!this->timer)
    {
        this->timer = new QTimer();
        this->timer->setSingleShot(false);
        this->timer->setInterval(this->wdInterval_ms);
        QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(watchdogTick()));
    }
    timer->start();
}

void WidgetVoltage::watchdogDisable()
{
    if(this->timer)
    {
        timer->stop();
    }
}

int WidgetVoltage::getInputVoltage()
{
    return ui->lineEdit_voltageInput->text().toInt();
}

void WidgetVoltage::setTCPClient(RSCANDoseClient *client)
{
    this->client = client;
}

#include <iostream>
void WidgetVoltage::setEnabled(bool enabled)
{
    QWidget::setEnabled(enabled);
    if(enabled)
    {
        // polarity




        watchdogEnable();
        std::cout << "enabled\n";
    }
    else
    {
        watchdogDisable();
        // buttons style

        std::cout << "disabled\n";
    }
}

void WidgetVoltage::setDisabled(bool disabled)
{
    this->WidgetVoltage::setEnabled(!disabled);
}

void WidgetVoltage::watchdogTick()
{
}

void WidgetVoltage::on_pushButton_changeVoltage_clicked()
{
    /*int maxVoltage = 500;
    int targetVoltage = ui->lineEdit_voltageInput->text().toInt();
    if(this->transmitter)
    {
        if(targetVoltage < 0 || targetVoltage > maxVoltage) // check range
        {
            // bad value
            QMessageBox::critical(this,
                                  "Ошибка!",
                                  "Допустимый диапазон:\nV мин. = 0 В\nV макс. = " + QString::number(maxVoltage) + " B"
                                  );
        }
        else
        {
            // good value
            this->transmitter->setVoltageValue(static_cast<uint16_t>(targetVoltage));
        }
    } */
}

void WidgetVoltage::on_pushButton_plus_clicked()
{

}

void WidgetVoltage::on_pushButton_minus_clicked()
{

}

void WidgetVoltage::setEnabledStyle(QPushButton *button, const QIcon &icon, int iconSize)
{

}

void WidgetVoltage::setDisabledStyle(QPushButton *button, const QIcon &icon, int iconSize)
{

}

void WidgetVoltage::setBlockedStyle(QPushButton *button, const QIcon &icon, int iconSize)
{

}
