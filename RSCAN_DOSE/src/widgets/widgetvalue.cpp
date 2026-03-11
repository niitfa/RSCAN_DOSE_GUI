#include "widgetvalue.h"
#include "ui_widgetvalue.h"

WidgetValue::WidgetValue(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetValue)
{
    ui->setupUi(this);

    setValueText("0");
    setHeadText("Description, unit");

    /*QFont headFont;
    headFont.setFamily("Inter");
    headFont.setPixelSize(16);
    headFont.setWeight(40);

    QFont valueFont;
    valueFont.setFamily("Inter");
    valueFont.setPixelSize(25); // 28
    valueFont.setWeight(60); */

    QFont headFont;
    headFont.setFamily("Inter");
    headFont.setPixelSize(16);
    headFont.setWeight(55);

    QFont valueFont;
    valueFont.setFamily("Inter");
    valueFont.setPixelSize(40); // 28
    valueFont.setWeight(60);

    // border
    ui->frame_border->setStyleSheet(
                //" QFrame { background-color: white; } "
                " QFrame { background-color: rgb(247,247,247); } "
                " QFrame { border-radius:10px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(180,180,180); }"
                // disabled
                " QFrame:disabled { background-color: rgb(247,247,247); } "
                );

    ui->verticalLayout_border->setParent(nullptr);
    ui->frame_border->setLayout(ui->verticalLayout_border);

    this->enabledColorText =  "color: rgb(50,70,100);";
    this->disabledColorText = "color: rgb(130,130,130);";

    // head text
    ui->label_head->setFont(headFont);
    // value text
    ui->label_value->setFont(valueFont);
    // init style
    this->WidgetValue::setEnabled(true);
}

WidgetValue::~WidgetValue()
{
    delete ui;
}

void WidgetValue::setHeadText(QString qstr)
{
    ui->label_head->setText(qstr);
}

void WidgetValue::setValueText(QString qstr)
{
    ui->label_value->setText(qstr);
}

void WidgetValue::setEnabled(bool enabled)
{
    QWidget::setEnabled(enabled);
    if(enabled)
    {
        ui->label_head->setStyleSheet(
            "border-width:0px;" +
            this->enabledColorText
        );
        ui->label_value->setStyleSheet(
            "border-width:0px;" +
            this->enabledColorText
        );
    }
    else
    {
        ui->label_head->setStyleSheet(
            "border-width:0px;" +
            this->disabledColorText
        );
        ui->label_value->setStyleSheet(
            "border-width:0px;" +
            this->disabledColorText
        );
    }
}
void WidgetValue::setDisabled(bool disabled)
{
    this->WidgetValue::setEnabled(!disabled);    
}
