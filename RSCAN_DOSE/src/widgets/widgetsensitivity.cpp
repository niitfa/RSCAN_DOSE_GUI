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
}

WidgetSensitivity::~WidgetSensitivity()
{
    delete ui;
}


void WidgetSensitivity::setTCPClient(RSCANDoseClient *client)
{
    this->client = client;
}

void WidgetSensitivity::setCommandCodes(RSCANDoseCommandCode setSensitivityCode)
{
    this->setSensitivityCode = setSensitivityCode;
}

void WidgetSensitivity::setValueCodes(RSCANDoseValueCode getSensitivityCode)
{
    this->getSensitivityCode = getSensitivityCode;
}

void WidgetSensitivity::setEnabled(bool enabled)
{
   QWidget::setEnabled(enabled);
   if(client)
   {
       if(enabled)
       {
            this->setupSensitivityButtons(client->readValue(this->getSensitivityCode));
            this->previousSensitivity = client->readValue(this->getSensitivityCode);
            watchdogEnable();
       }
       else
       {
            this->setupSensitivityButtons(client->readValue(this->getSensitivityCode));
            this->previousSensitivity = client->readValue(this->getSensitivityCode);
            watchdogDisable();
       }
   }
}

void WidgetSensitivity::setDisabled(bool disabled)
{
    this->WidgetSensitivity::setEnabled(!disabled);
}

void WidgetSensitivity::watchdogTick()
{
    if(client)
    {
        // check sensitivity
        bool sensitivity = client->readValue(this->getSensitivityCode);
        if(sensitivity != this->previousSensitivity)
        {
            this->setupSensitivityButtons(sensitivity);
            this->previousSensitivity = sensitivity;
        }
    }

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


    QString headTextColor = "color: rgb(70,70,70);";
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

void WidgetSensitivity::watchdogEnable()
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

void WidgetSensitivity::watchdogDisable()
{
    if(this->timer)
    {
        timer->stop();
    }
}

void WidgetSensitivity::setupSensitivityButtons(bool sensitivity)
{
    if(sensitivity)
    {
        this->setLowButtonActive(true);
        this->setHighButtonActive(false);
    }
    else
    {
        this->setLowButtonActive(false);
        this->setHighButtonActive(true);
    }
}

void WidgetSensitivity::on_pushButton_sensitivityLow_clicked()
{
    if(client)
    {
        client->sendCommand(this->setSensitivityCode, 0);
    }
}

void WidgetSensitivity::on_pushButton_sensitivityHigh_clicked()
{
    if(client)
    {
        client->sendCommand(this->setSensitivityCode, 1);
    }
}

void WidgetSensitivity::setInactiveStyle(QPushButton *button)
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

                // disabled
                "QPushButton:disabled { border-width:0px;  }"
                "QPushButton:disabled { background-color: rgb(200,200,200);  }"
                "QPushButton:disabled { color: white;  }"
                );
    }

}

void WidgetSensitivity::setActiveStyle(QPushButton *button)
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

                // disabled
                "QPushButton:disabled { border-width:0px;  }"
                "QPushButton:disabled { background-color: rgb(255,255,255);  }"
                "QPushButton:disabled { color: rgb(200,200,200);  }"
                );
    }
}

void WidgetSensitivity::setLowButtonActive(bool active)
{
    QPushButton* button = ui->pushButton_sensitivityLow;
    if(active)
    {
        this->setActiveStyle(button);
    }
    else
    {
        this->setInactiveStyle(button);
    }
}

void WidgetSensitivity::setHighButtonActive(bool active)
{
    QPushButton* button = ui->pushButton_sensitivityHigh;
    if(active)
    {
        this->setActiveStyle(button);
    }
    else
    {
        this->setInactiveStyle(button);
    }
}

