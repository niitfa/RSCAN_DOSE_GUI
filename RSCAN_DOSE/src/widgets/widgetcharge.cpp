#include "widgetcharge.h"
#include "ui_widgetcharge.h"

WidgetCharge::WidgetCharge(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetCharge)
{
    ui->setupUi(this);
}

WidgetCharge::~WidgetCharge()
{
    delete ui;
}

void WidgetCharge::setClient(RSCANDoseClient *cli)
{
    client = cli;
}

void WidgetCharge::init()
{

}
