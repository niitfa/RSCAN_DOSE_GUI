#include "widgetvaluechecked.h"
#include "ui_widgetvaluechecked.h"
#include "movingaverage.h"

WidgetValueChecked::WidgetValueChecked(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetValueChecked)
{
    ui->setupUi(this);

    setValueText("0");
    setHeadText("Description, unit");

    QFont headFont;
    headFont.setFamily("Inter");
    headFont.setPixelSize(15);
    headFont.setWeight(55);

    QFont valueFont;
    valueFont.setFamily("Inter");
    valueFont.setPixelSize(25); // 28
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
    // label count
    ui->label_count->setFont(headFont);
    // check box graph text
    ui->checkBox->setFont(headFont);
    ui->checkBox->setText("График");

    ui->checkBox_averaging->setFont(headFont);
    ui->checkBox_averaging->setText("Усредн.");

    // init style
    this->WidgetValueChecked::setEnabled(true);
    
}

WidgetValueChecked::~WidgetValueChecked()
{
    delete ui;
}

void WidgetValueChecked::setHeadText(QString qstr)
{
    ui->label_head->setText(qstr);
}

void WidgetValueChecked::setValueText(QString qstr)
{
    ui->label_value->setText(qstr);
}

void WidgetValueChecked::setCheckedCallback(std::function<void (bool)> cb)
{
    this->checkedCallback = cb;
}

void WidgetValueChecked::setChecked(bool checked)
{
    ui->checkBox->setChecked(checked);
    checkedCallback(checked);
}

void WidgetValueChecked::setMovingAverage(MovingAverage *av)
{
    this->average = av;
    if(average)
    {
        ui->lineEdit_averagingLevel->setText(QString::number(average->getAveragingLevel()));
    }
}

void WidgetValueChecked::setEnabled(bool enabled)
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
        ui->label_count->setStyleSheet(
            "border-width:0px;" +
            this->enabledColorText
        );
        ui->checkBox->setStyleSheet(
            "border-width:0px;" +
            this->enabledColorText
        );
        ui->checkBox_averaging->setStyleSheet(
            "border-width:0px;" +
            this->enabledColorText
        );
        ui->lineEdit_averagingLevel->setStyleSheet(
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
        ui->label_count->setStyleSheet(
            "border-width:0px;" +
            this->disabledColorText
        );
        ui->checkBox->setStyleSheet(
            "border-width:0px;" +
            this->disabledColorText
        );
        ui->checkBox_averaging->setStyleSheet(
            "border-width:0px;" +
            this->disabledColorText
        );
        ui->lineEdit_averagingLevel->setStyleSheet(
            "border-width:0px;" +
            this->disabledColorText
        );
    }
}

void WidgetValueChecked::setDisabled(bool disabled)
{
    this->WidgetValueChecked::setEnabled(!disabled);
}

void WidgetValueChecked::on_checkBox_clicked()
{
    checkedCallback(ui->checkBox->isChecked());
}

void WidgetValueChecked::on_checkBox_averaging_clicked()
{
    if(average)
    {
        if(ui->checkBox_averaging->isChecked())
        {
            average->setAveragingLevel( ui->lineEdit_averagingLevel->text().toInt() );
        }
        else
        {
            average->setAveragingLevel(1);
        }
    }
}

void WidgetValueChecked::on_lineEdit_averagingLevel_editingFinished()
{
    bool ok = false;
    int averagingLevel = ui->lineEdit_averagingLevel->text().toInt(&ok);
    if(ok)
    {
        if(averagingLevel < 1)
        {
            averagingLevel = 1;
            ui->lineEdit_averagingLevel->setText(QString::number(averagingLevel));
        }
        if(average && (averagingLevel >= average->maxSize))
        {
            averagingLevel = average->maxSize;
            ui->lineEdit_averagingLevel->setText(QString::number(averagingLevel));
        }
        if(average && ui->checkBox_averaging->isChecked())
        {
            average->setAveragingLevel(averagingLevel);
        }
    }
    else
    {
        if(average)
        {
            ui->lineEdit_averagingLevel->setText(QString::number(average->getAveragingLevel()));
        }
    }

}
