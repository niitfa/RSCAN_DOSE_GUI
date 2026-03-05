#include "widgetsensitivity.h"
#include "ui_widgetsensitivity.h"

WidgetSensitivity::WidgetSensitivity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSensitivity)
{
    ui->setupUi(this);

    drawInternalFrame();
    drawExternalFrame();
    drawIcon();
    drawHeadText();
    setupButtonsStyle();

    setSensitivity(!this->lastSensitivity);
}

WidgetSensitivity::~WidgetSensitivity()
{
    delete ui;
}

void WidgetSensitivity::setSensitivity(uint8_t sensitivity)
{
    if((sensitivity != this->lastSensitivity))
    {
        if(sensitivity)
        {
            this->setHighButtonDisabled();
            this->setLowButtonEnabled();
        }
        else
        {
            this->setLowButtonDisabled();
            this->setHighButtonEnabled();

        }
        this->lastSensitivity = sensitivity;
    }
}

void WidgetSensitivity::setTCPClient(RSCANDoseClient *client)
{
    this->client = client;
}

void WidgetSensitivity::setEnabledStyle(QPushButton *button)
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
                "QPushButton { color: rgb(20,20,20); }"
                // hover
                "QPushButton:hover { background-color: rgb(230,230,230);  }"
                // pressed
                "QPushButton:pressed { background-color: rgb(200,200,200);  }"
                );
    }
}

void WidgetSensitivity::setDisabledStyle(QPushButton *button)
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
                "QPushButton { color: white; }"
                );
    }
}

void WidgetSensitivity::setLowButtonEnabled()
{
    QPushButton* button = ui->pushButton_sensitivityLow;
    button->setEnabled(1);
    button->setText("Низкая");
    this->setEnabledStyle(button);
}

void WidgetSensitivity::setLowButtonDisabled()
{
    QPushButton* button = ui->pushButton_sensitivityLow;
    button->setDisabled(1);
    button->setText("Низкая");
    this->setDisabledStyle(button);
}

void WidgetSensitivity::setHighButtonEnabled()
{
    QPushButton* button = ui->pushButton_sensitivityHigh;
    button->setEnabled(1);
    button->setText("Высокая");
    this->setEnabledStyle(button);
}

void WidgetSensitivity::setHighButtonDisabled()
{
    QPushButton* button = ui->pushButton_sensitivityHigh;
    button->setDisabled(1);
    button->setText("Высокая");
    this->setDisabledStyle(button);
}

void WidgetSensitivity::drawInternalFrame()
{
    ui->frame_borderInternal->setStyleSheet(
            " QFrame { background-color: rgb(247,247,247); } "
            " QFrame { border-radius:10px; }"
            " QFrame { border-style: outset; }"
            " QFrame { border-width:1px; }"
            " QFrame { border-color:rgb(150,150,150); }"
            );

    ui->horizontalLayout_borderInternal->setParent(nullptr);
    ui->frame_borderInternal->setLayout(ui->horizontalLayout_borderInternal);
}

void WidgetSensitivity::drawExternalFrame()
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

void WidgetSensitivity::drawIcon()
{
    int headIconSize = 20;
    QIcon headIcon(":/img/icon_sensivity.png");
    ui->label_headIcon->clear();
    ui->label_headIcon->setPixmap(headIcon.pixmap(headIconSize,headIconSize));
    ui->label_headIcon->setStyleSheet(
                "border-width:0px;"
                );
}

void WidgetSensitivity::drawHeadText()
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
    ui->label_headText->setText("Чувствительность");
}

void WidgetSensitivity::setupButtonsStyle()
{
    buttonsFont.setFamily("Inter");
    buttonsFont.setPixelSize(15);
    buttonsFont.setWeight(50);

    ui->pushButton_sensitivityLow->setFont(buttonsFont);
    ui->pushButton_sensitivityLow->setText("Низкая");
    ui->pushButton_sensitivityHigh->setFont(buttonsFont);
    ui->pushButton_sensitivityHigh->setText("Высокая");
}

void WidgetSensitivity::on_pushButton_sensitivityLow_clicked()
{
    /*if(transmitter)
    {
        transmitter->setBroadRange();
        //setSensivity(0); // debug!!!!
    } */
}

void WidgetSensitivity::on_pushButton_sensitivityHigh_clicked()
{
    /* if(transmitter)
    {
        transmitter->setNarrowRange();
        //setSensivity(1); // debug!!!!
    } */
}

