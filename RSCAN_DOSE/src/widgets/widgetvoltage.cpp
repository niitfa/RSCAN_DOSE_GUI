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
                "QPushButton:disabled { color: white;  }"
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

void WidgetVoltage::setActiveWidgetsEnabled(bool enabled)
{
    ui->pushButton_changeVoltage->setEnabled(enabled);
    ui->lineEdit_voltageInput->setEnabled(enabled);
    ui->label_value->setEnabled(enabled);
    ui->pushButton_minus->setEnabled(enabled);
    ui->pushButton_plus->setEnabled(enabled);
}

void WidgetVoltage::setupWidgetsHVEnabled(bool enabled)
{
    if(enabled)
    {
        this->setActiveWidgetsEnabled(true);
        if(!ui->switch_hvEnable->isChecked())
        {
            ui->switch_hvEnable->setChecked(true);
        }

    }
    else
    {
        this->setActiveWidgetsEnabled(false);
        if(ui->switch_hvEnable->isChecked())
        {
            ui->switch_hvEnable->setChecked(false);
        }
    }

}

void WidgetVoltage::setupWidgetsPolarity(bool polarity)
{
    if(polarity) // -
    {
        this->setMinusButtonActive(false);
        this->setPlusButtonActive(true);
    }
    else // +
    {
        this->setMinusButtonActive(true);
        this->setPlusButtonActive(false);
    }
}

void WidgetVoltage::enablePolarityButtons(bool enabled)
{
    if((ui->pushButton_plus->isEnabled() != enabled))
    {
        ui->pushButton_plus->setEnabled(enabled);
    }

    if((ui->pushButton_minus->isEnabled() != enabled))
    {
        ui->pushButton_minus->setEnabled(enabled);
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

void WidgetVoltage::setCommandCodes(
        RSCANDoseCommandCode hvEnableCode,
        RSCANDoseCommandCode hvDisableCode,
        RSCANDoseCommandCode setVoltageCode,
        RSCANDoseCommandCode setPolarityCode
        )
{
    this->hvEnableCode = hvEnableCode;
    this->hvDisableCode = hvDisableCode;
    this->setVoltageCode = setVoltageCode;
    this->setPolarityCode = setPolarityCode;
}

void WidgetVoltage::setValueCodes(
        RSCANDoseValueCode getDoseCode,
        RSCANDoseValueCode getHVCode,
        RSCANDoseValueCode getTemperatureCode,
        RSCANDoseValueCode getSensitivityCode,
        RSCANDoseValueCode getPolarityCode,
        RSCANDoseValueCode getHVEnabledCode
        )
{
    this->getDoseCode = getDoseCode;
    this->getHVCode = getHVCode;
    this->getTemperatureCode = getTemperatureCode;
    this->getSensitivityCode = getSensitivityCode;
    this->getPolarityCode = getPolarityCode;
    this->getHVEnabledCode = getHVEnabledCode;
}

void WidgetVoltage::setEnabled(bool enabled)
{
    QWidget::setEnabled(enabled);
    if(client)
    {
        if(enabled)
        {
            //  voltage
            this->setupWidgetsHVEnabled(client->readValue(this->getHVEnabledCode));
            this->previousHVEnabledState = client->readValue(this->getHVEnabledCode);

            // polarity
            this->setupWidgetsPolarity(client->readValue(this->getPolarityCode));
            this->previousPolarity = client->readValue(this->getPolarityCode);

            // enable wd
            watchdogEnable();
        }
        else
        {
            // voltage
            // polarity
            this->setupWidgetsPolarity(client->readValue(this->getPolarityCode));
            this->previousPolarity = client->readValue(this->getPolarityCode);

            // enable wd
            watchdogDisable();
        }
    }
}

void WidgetVoltage::setDisabled(bool disabled)
{
    this->WidgetVoltage::setEnabled(!disabled);
}

void WidgetVoltage::watchdogTick()
{
    if(client)
    {
        // check voltage enabled
        bool hvEnabled = client->readValue(this->getHVEnabledCode);
        if(hvEnabled != this->previousHVEnabledState)
        {
            this->setupWidgetsHVEnabled(hvEnabled);
            this->previousHVEnabledState = hvEnabled;
        }

        // check polarity
        bool polarity = client->readValue(this->getPolarityCode);
        if(polarity != this->previousPolarity)
        {
            this->setupWidgetsPolarity(polarity);
            this->previousPolarity = polarity;
        }

        // check polarity voltage limit
        int voltage = client->readValue(this->getHVCode);
        if(hvEnabled)
        {
            if(qAbs(voltage) < this->polaritySwitchLimitVoltage)
            {
                this->enablePolarityButtons(true);
            }
            else
            {
                this->enablePolarityButtons(false);
            }
        }
    }
}

void WidgetVoltage::on_pushButton_changeVoltage_clicked()
{
    int targetVoltage = ui->lineEdit_voltageInput->text().toInt();
    if(client)
    {
        if(targetVoltage < 0 || targetVoltage > this->maxVoltage) // check range
        {
            // bad value
            QMessageBox::critical(this,
                                  "Ошибка!",
                                  "Допустимый диапазон:\nV мин. = 0 В\nV макс. = " + QString::number(maxVoltage) + " B"
                                  );
            ui->lineEdit_voltageInput->setText(QString::number(0));
        }
        else
        {
            // good value
            client->sendCommand(this->setVoltageCode, targetVoltage);
        }
    }
}

void WidgetVoltage::on_pushButton_plus_clicked()
{
    if(client)
    {
        client->sendCommand(this->setPolarityCode, 0);
    }
}

void WidgetVoltage::on_pushButton_minus_clicked()
{
    if(client)
    {
        client->sendCommand(this->setPolarityCode, 1);
    }
}

void WidgetVoltage::setInactiveStyle(QPushButton *button, const QIcon &icon, int iconSize)
{
    if(button)
    {
        button->setText("");
        button->setIcon(icon);
        button->setIconSize(QSize(iconSize, iconSize));

        button->setStyleSheet(
                // unpressed
                "QPushButton { border-style: outset; }"
                "QPushButton { border-radius:5px; }"
                "QPushButton { border-width:1px; }"
                "QPushButton { border-color: rgb(50,100,210); }"
                "QPushButton { background-color: rgb(60,120,230); }"

                // disabled
                "QPushButton:disabled { border-width:0px;  }"
                "QPushButton:disabled { background-color: rgb(200,200,200);  }"
                );
    }
}

void WidgetVoltage::setActiveStyle(QPushButton *button, const QIcon &icon, int iconSize)
{
    if(button)
    {
        button->setText("");
        button->setIcon(icon);
        button->setIconSize(QSize(iconSize, iconSize));

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
                );
    }

}

void WidgetVoltage::setMinusButtonActive(bool active)
{
    QPushButton* button = ui->pushButton_minus;
    QIcon icon;
    int iconSize = 25;
    if(active)
    {
        icon = QIcon(":/img/icon_minus_dark.png");
        this->setActiveStyle(button, icon, iconSize);
    }
    else
    {
        icon = QIcon(":/img/icon_minus_bright.png");
        this->setInactiveStyle(button, icon, iconSize);
    }
}

void WidgetVoltage::setPlusButtonActive(bool active)
{
    QPushButton* button = ui->pushButton_plus;
    QIcon icon;
    int iconSize = 22;
    if(active)
    {
        icon = QIcon(":/img/icon_plus_dark.png");
        this->setActiveStyle(button, icon, iconSize);
    }
    else
    {
        icon = QIcon(":/img/icon_plus_bright.png");
        this->setInactiveStyle(button, icon, iconSize);
    }
}

void WidgetVoltage::on_switch_hvEnable_clicked()
{
    if(ui->switch_hvEnable->isChecked())
    {
        if(client)
        {
            client->sendCommand(this->hvEnableCode);
        }
    }
    else
    {
        if(client)
        {
            client->sendCommand(this->hvDisableCode);
        }
    }
}
