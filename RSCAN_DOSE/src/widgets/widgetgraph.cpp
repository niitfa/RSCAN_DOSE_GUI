#include "widgetgraph.h"
#include "ui_widgetgraph.h"

WidgetGraph::WidgetGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetGraph)
{
    ui->setupUi(this);

    setupFont();
    setupGraph();
    setupStartButton();
    setupTexts();
    setupLineEdits();

    this->WidgetGraph::setEnabled(true);
}

WidgetGraph::~WidgetGraph()
{
    delete graph;
    delete ui;
}

void WidgetGraph::setEnabled(bool enabled)
{
    QWidget::setEnabled(enabled);
    setEnabledTextColor(enabled);
    if(enabled)
    {
        // graph enabled
        ui->label_noData->hide();
        ui->widget_graph->show();
        graph->show();
        startGraph();
    }
    else
    {
        // graph disabled
        ui->label_noData->show();
        ui->widget_graph->hide();
        stopGraph();
    }
}

void WidgetGraph::setDisabled(bool disabled)
{
    this->WidgetGraph::setEnabled(!disabled);
}

void WidgetGraph::setLineVisible(int lineNo, bool visible)
{
    if(visible)
    {
        this->graph->showGraph(lineNo);
    }
    else
    {
        this->graph->hideGraph(lineNo);
    }
}

void WidgetGraph::pushData(const QVector<double> &data)
{
    if(this->graph)
    {
        graph->update(data);
    }
}

void WidgetGraph::setStartStyle(QPushButton * button, QString text)
{
    QIcon icon(":/img/button_icon_start.png");
    int iconSize = 14;
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize,iconSize));
    button->setFont(buttonsFont);
    button->setText(text);

    button->setStyleSheet(
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
                ); // Start */
    button->setFocusPolicy( Qt::FocusPolicy::NoFocus );
}

void WidgetGraph::setStopStyle(QPushButton * button, QString text)
{
    QIcon icon(":/img/button_icon_pause.png");
    int iconSize = 24;
    button->setIcon(icon);
    button->setIconSize(QSize(iconSize,iconSize));
    button->setFont(buttonsFont);
    button->setText(text);

    button->setStyleSheet(
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
                ); // Start */
    button->setFocusPolicy( Qt::FocusPolicy::NoFocus );
}

void WidgetGraph::setEnabledTextColor(bool enabled)
{
    QString color = enabled ? enabledColorText : disabledColorText;
    ui->label_maxSignal->setStyleSheet("border-width:0px;" + color);
    ui->label_minSignal->setStyleSheet("border-width:0px;" + color);
    ui->label_time->setStyleSheet("border-width:0px;" + color);
    ui->lineEdit_graphMax->setStyleSheet("border-width:0px;" + color);
    ui->lineEdit_graphMin->setStyleSheet("border-width:0px;" + color);
    ui->lineEdit_time->setStyleSheet("border-width:0px;" + color);
}

void WidgetGraph::startGraph()
{
    if(this->graph)
    {
        this->graph->setEnabled(true);
        this->setStopStyle(ui->pushButton_startGraph, graphButtonStopText);
        ui->lineEdit_time->setDisabled(1);
    }
}

void WidgetGraph::stopGraph()
{
    if(this->graph)
    {
        this->graph->setEnabled(false);
        this->setStartStyle(ui->pushButton_startGraph, graphButtonStartText);
        ui->lineEdit_time->setEnabled(1);
    }
}

void WidgetGraph::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(this->graph)
    {
        this->graph->show();
    }
}

void WidgetGraph::on_pushButton_startGraph_clicked()
{
    if(this->graphStarted)
    {
        stopGraph();
    }
    else
    {
        startGraph();
    }
    this->graphStarted = !this->graphStarted;
}

void WidgetGraph::on_lineEdit_graphMin_editingFinished()
{
    double min = ui->lineEdit_graphMin->text().toDouble();
    double currMax = this->graph->getYMax();
    if(min < currMax)
    {
        this->graph->setYAxisRange(min, currMax);
    }
    else
    {
        int currMinInt = static_cast<int>(this->graph->getYMin());
        ui->lineEdit_graphMin->setText(
                    QString::fromStdString(std::to_string(currMinInt))
                    );
    }
}

void WidgetGraph::on_lineEdit_graphMax_editingFinished()
{
    double max = ui->lineEdit_graphMax->text().toDouble();
    double currMin = this->graph->getYMin();
    if(max > currMin)
    {
        this->graph->setYAxisRange(currMin, max);
    }
    else
    {
        int currMaxInt = static_cast<int>(this->graph->getYMax());
        ui->lineEdit_graphMax->setText(
                    QString::fromStdString(std::to_string(currMaxInt))
                    );
    }
}

void WidgetGraph::on_lineEdit_time_editingFinished()
{
    double tRange = ui->lineEdit_time->text().toDouble();
    if(tRange > 0)
    {

        this->graph->setTAxisRange(0, tRange);
    }
    else
    {
         ui->lineEdit_time->setText(
                     QString::number(this->graph->getTimeRange(), 'f', 1)
                     );
    }

}

void WidgetGraph::setupStartButton()
{
    this->graphStarted = 1;
    this->graphButtonStartText = "Возобновить";
    this->graphButtonStopText = "Остановить";
    this->setStopStyle(ui->pushButton_startGraph, graphButtonStopText);
}

void WidgetGraph::setupFont()
{
    buttonsFont.setFamily("Inter");
    buttonsFont.setPixelSize(15);
    buttonsFont.setWeight(50);

    this->enabledColorText =  "color: rgb(50,70,100);";
    this->disabledColorText = "color: rgb(130,130,130);";
}

void WidgetGraph::setupTexts()
{
    ui->label_maxSignal->setFont(buttonsFont);
    ui->label_minSignal->setFont(buttonsFont);
    ui->label_time->setFont(buttonsFont);
    ui->lineEdit_graphMax->setFont(buttonsFont);
    ui->lineEdit_graphMin->setFont(buttonsFont);
    ui->lineEdit_time->setFont(buttonsFont);


    QFont font;
    font.setFamily("Inter");
    font.setPixelSize(24);
    font.setWeight(50);
    ui->label_noData->setFont(font);
    ui->label_noData->setText("Нет данных");
    ui->label_noData->setStyleSheet("border-width:0px;" + disabledColorText);
}

void WidgetGraph::setupGraph()
{
    this->graph = new QGraph(ui->widget_graph);
    this->graph->setTAxisRange(0, static_cast<double>(this->tGraphRange));
    this->graph->setYAxisRange(this->yGraphMinRange, this->yGraphMaxRange);

    ui->frame_border->setStyleSheet(
                " QFrame { background-color: rgb(247,247,247); } "
                " QFrame { border-radius:30px; }"
                " QFrame { border-style: outset; }"
                " QFrame { border-width:1px; }"
                " QFrame { border-color:rgb(150,150,150); }"
                );

    ui->horizontalLayout_graph->setParent(nullptr);
    ui->frame_border->setLayout(ui->horizontalLayout_graph);
}

void WidgetGraph::setupLineEdits()
{
    ui->lineEdit_graphMax->setText(QString::number(this->yGraphMaxRange));
    ui->lineEdit_graphMin->setText(QString::number(this->yGraphMinRange));
    ui->lineEdit_time->setText(QString::number(this->tGraphRange));
}
