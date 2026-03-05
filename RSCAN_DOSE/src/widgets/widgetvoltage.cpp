#include "widgetvoltage.h"
#include "ui_widgetvoltage.h"
#include <QIcon>
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
    setPolarity(!this->lastPolarity);
}

WidgetVoltage::~WidgetVoltage()
{
    delete ui;
}

void WidgetVoltage::setEnabledStyle(QPushButton * button)
{
    if(button)
    {
        button->setStyleSheet(
                // unpressed
                "QPushButton { border-style: outset; }"
                "QPushButton { border-radius:5px; }"
                "QPushButton { border-width:1px; }"
                "QPushButton { border-color: rgb(220,220,220); }"
                "QPushButton { background-color: rgb(240,240,240); }"
                // hover
                "QPushButton:hover { background-color: rgb(230,230,230);  }"
                // pressed
                "QPushButton:pressed { background-color: rgb(200,200,200);  }"
                );
    }
}

void WidgetVoltage::setDisabledStyle(QPushButton * button)
{
    if(button)
    {
        button->setStyleSheet(
                // unpressed
                "QPushButton { border-style: outset; }"
                "QPushButton { border-radius:5px; }"
                "QPushButton { border-width:1px; }"
                "QPushButton { border-color: rgb(50,100,210); }"
                "QPushButton { background-color: rgb(60,120,230); }"
                );
    }
}

void WidgetVoltage::setInactiveStyle(QPushButton* button)
{
    if(button)
    {
        button->setStyleSheet(
                // unpressed
                "QPushButton { border-style: outset; }"
                "QPushButton { border-radius:5px; }"
                "QPushButton { border-width:0px; }"
                "QPushButton { background-color: rgb(129,142,172); }"
                );
    }
}

void WidgetVoltage::setPlusButtonEnabled()
{
    QPushButton* button = ui->pushButton_plus;
    button->setEnabled(1);
    QIcon icon (":/img/icon_plus_dark.png");
    int iconSize = 22;
    button->setText("");
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize, iconSize));
    this->setEnabledStyle(button);
}

void WidgetVoltage::setPlusButtonDisabled()
{
    QPushButton* button = ui->pushButton_plus;
    button->setDisabled(1);
    QIcon icon (":/img/icon_plus_bright.png");
    int iconSize = 22;
    button->setText("");
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize, iconSize));
    this->setDisabledStyle(button);
}

void WidgetVoltage::setMinusButtonEnabled()
{
    QPushButton* button = ui->pushButton_minus;
    button->setEnabled(1);
    QIcon icon (":/img/icon_minus_dark.png");
    int iconSize = 25;
    button->setText("");
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize, iconSize));
    this->setEnabledStyle(button);
}

void WidgetVoltage::setMinusButtonDisabled()
{
    QPushButton* button = ui->pushButton_minus;
    button->setDisabled(1);
    QIcon icon (":/img/icon_minus_bright.png");
    int iconSize = 25;
    button->setText("");
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize, iconSize));
    this->setDisabledStyle(button);
}

void WidgetVoltage::setPolarity(uint8_t polarity)
{
    if((polarity != this->lastPolarity))
    {
        if(polarity) // negative
        {
            this->setMinusButtonDisabled();
            this->setPlusButtonEnabled();
        }
        else // positive
        {
            this->setPlusButtonDisabled();
            this->setMinusButtonEnabled();

        }
        this->lastPolarity = polarity;
    }
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

    QString headTextColor = "color: black;";

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
                );
}

void WidgetVoltage::setupLineEditVoltage()
{
    // label Voltage value measurements
    ui->label_value->setStyleSheet(
                "border-width:0px;"
                );
    ui->label_value->setFont(buttonsFont);
    ui->label_value->setText("В");

    // line edit voltage input
    ui->lineEdit_voltageInput->setStyleSheet(
                "border-width:0px;"
                );
    ui->lineEdit_voltageInput->setFont(buttonsFont);
    ui->lineEdit_voltageInput->setText(QString::number(0));
}

int WidgetVoltage::getInputVoltage()
{
    return ui->lineEdit_voltageInput->text().toInt();
}

void WidgetVoltage::setTCPClient(RSCANDoseClient *client)
{
    this->client = client;
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
