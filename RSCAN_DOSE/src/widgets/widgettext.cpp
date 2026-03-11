#include "widgettext.h"
#include "ui_widgettext.h"

WidgetText::WidgetText(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetText)
{
    ui->setupUi(this);

    drawFrame();
    setupFont();
}

WidgetText::~WidgetText()
{
    delete ui;
}

void WidgetText::drawFrame()
{
    ui->frame_border->setStyleSheet(
                " QFrame { background-color: rgb(230,230,230); } "
                " QFrame { border-radius:15px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );

    ui->horizontalLayout->setParent(nullptr);
    ui->frame_border->setLayout(ui->horizontalLayout);
}

void WidgetText::setupFont()
{
    font.setFamily("Inter");
    font.setPixelSize(24);
    font.setWeight(60);

    ui->label->setFont(font);
    ui->label->setStyleSheet(
                "border-width:0px;"
                "color: rgb(50,70,100);"
            );
}

void WidgetText::setText(const QString &text)
{
    ui->label->setText(text);
}

