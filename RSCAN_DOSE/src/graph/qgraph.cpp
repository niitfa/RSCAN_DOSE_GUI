#include "qgraph.h"
#include "ui_qgraph.h"
#include <iostream>

QGraph::QGraph(QWidget *parent) :
    QCustomPlot(parent),
    ui(new Ui::QGraph)
{
    connect(this, SIGNAL(mouseMove(QMouseEvent*)), this,SLOT(showPointToolTip(QMouseEvent*)));
    ui->setupUi(this);
    this->setupStyles();
    this->setupGraph();

    // init data
    this->yVec = QVector<QVector<double>>(numberOfGraphs);
    this->QCustomPlot::legend->setVisible(true);
    hideAllGraphs();
}

QGraph::~QGraph()
{
    delete ui;
}

QVector<double> QGraph::back()
{
    if(this->yVec.isEmpty())
    {
        return QVector<double>();
    }

    QVector<double> ret;
    for(int i = 0; i < numberOfGraphs; i++)
    {
        ret.push_back(this->yVec[i].back());
    }

    return ret;
}

void QGraph::show()
{
    int width = parentWidget()->geometry().width();
    int height = parentWidget()->geometry().height();
    this->setGeometry(0, 0, width, height);
    this->QCustomPlot::show();
}

void QGraph::update(const QVector<double>& data)
{
    if(this->enabled)
    {
        // get current timе
        int msecs_elapsed = timer.restart();
        double secs_elapsed = static_cast<double>(msecs_elapsed) / 1000;
        this->updateTimeVector(secs_elapsed);
        this->yVecPushBack(data);

        while(this->isLimitTimeExceeded())
        {
            tMin += secs_elapsed;
            tMax += secs_elapsed;
            this->tVec.pop_front();
            this->yVecPopFront();
            this->setTAxisRange(tMin, tMax);
        }
        this->QGraph::replot();
    }
}

void QGraph::resizeYAxis()
{
    // max yVec, min yVec
}

void QGraph::setYAxisRange(double yMin, double yMax)
{
    this->yMin = yMin;
    this->yMax = yMax;
    this->QCustomPlot::yAxis->setRange(this->yMin, this->yMax);
    this->QGraph::replot();
}

void QGraph::setTAxisRange(double tMin, double tMax)
{
    this->tMin = tMin;
    this->tMax = tMax;
    this->QCustomPlot::xAxis->setRange(this->tMin, this->tMax);
    this->QGraph::replot();
}

void QGraph::updateTimeVector(double t)
{
    if(tVec.empty())
    {
        tVec.push_back(t);
    }
    else
    {
        tVec.push_back(t + this->tVec.back());
    }
}

bool QGraph::isLimitTimeExceeded()
{
    double limitTime = this->tMin + this->tFilled * (this->tMax - this->tMin);
    return this->tVec.back() > limitTime;
}

void QGraph::setEnabled(bool enabled)
{
    if(enabled)
    {
        if(!this->enabled)
        {
            this->resetData();
        }
    }
    this->enabled = enabled;
}

double QGraph::getTimeRange()
{
    return this->tMax - this->tMin;
}

double QGraph::getYMax()
{
    return this->yMax;
}

double QGraph::getYMin()
{
    return this->yMin;
}

void QGraph::resetData()
{
    this->tVec.clear();
    this->yVecClear();
    double tAxisRange = this->tMax - this->tMin;
    this->tMin = 0;
    this->tMax = tAxisRange;
    timer.restart();
    this->setTAxisRange(tMin, tMax);
}

void QGraph::setupGraph()
{
    QPen pen;
    QVector<QColor> colors = QVector<QColor>({
                    QColor(0x00, 0x66, 0x00), // 0:0
                    QColor(0x00, 0x66, 0xCC), // 0:1
               });

    /*for(int i = 0; i < this->numberOfGraphs; i++)
    {
        this->QCustomPlot::addGraph();
        pen.setColor(colors[i]);
        pen.setWidth(3);
        this->QCustomPlot::graph(i)->setPen(pen);
        this->QCustomPlot::graph(i)->setName(
                    QString::number(i / 4) + ":" + QString::number(i % 4)
                    );
    } */

    pen.setWidth(3);

    this->QCustomPlot::addGraph();
    pen.setColor(colors[0]);
    this->QCustomPlot::graph(0)->setPen(pen);
    this->QCustomPlot::graph(0)->setName("Канал A");

    this->QCustomPlot::addGraph();
    pen.setColor(colors[1]);
    this->QCustomPlot::graph(1)->setPen(pen);
    this->QCustomPlot::graph(1)->setName("Канал B");


}

void QGraph::removeGraph()
{
    int gCount = this->QCustomPlot::graphCount();
    for(int i = 0; i < gCount; i++)
    {
        this->QCustomPlot::removeGraph(0);
    }
}

void QGraph::replot()
{
    for(int i = 0; i < QCustomPlot::graphCount(); i++)
    {
        this->QCustomPlot::graph(i)->data()->clear(); // !!
        this->QCustomPlot::graph(i)->addData(this->tVec, this->yVec[i], true);
        this->QCustomPlot::replot();
    }
}

void QGraph::clearGraph()
{    
    for(int i = 0; i < QCustomPlot::graphCount(); i++)
    {
        this->QCustomPlot::graph(i)->data()->clear(); // !!
        this->QCustomPlot::replot();
    }
}

void QGraph::setupStyles()
{
    QFont font;
    font.setFamily("Inter");
    font.setPixelSize(15);
    font.setWeight(50);

    // labels
    this->QCustomPlot::xAxis->setLabel("Время, c");
    this->QCustomPlot::xAxis->setLabelFont(font);
    this->QCustomPlot::yAxis->setLabel("Сигнал, ед.");
    this->QCustomPlot::yAxis->setLabelFont(font);

    // background
    this->QCustomPlot::setBackground(QBrush(QColor("#f8f8f8")));
}

void QGraph::yVecPushBack(const QVector<double>& data)
{
    for(int i = 0; i < yVec.size(); i++)
    {
        if(i < data.size())
        {
            yVec[i].push_back(data.at(i));
        }
        else
        {
            yVec[i].push_back(0);
        }
    }
}

void QGraph::yVecPopFront()
{
    for(int i = 0; i < yVec.size(); i++)
    {
        yVec[i].pop_front();
    }
}

void QGraph::yVecClear()
{
    for(int i = 0; i < yVec.size(); i++)
    {
        yVec[i].clear();
    }
}

void QGraph::updateLegend()
{
    this->QCustomPlot::legend->clear();
    for(int i = 0; i < this->QCustomPlot::graphCount(); i++)
    {
        if(this->QCustomPlot::graph(i)->visible())
        {
            this->QCustomPlot::graph(i)->addToLegend(this->QCustomPlot::legend);
        }
    }
}

void QGraph::showGraph(int index)
{
    QCPGraph* g = this->QCustomPlot::graph(index);
    if(g && !g->visible())
    {
        g->setVisible(true);
        this->updateLegend();
    }
}

void QGraph::hideGraph(int index)
{
    QCPGraph* g = this->QCustomPlot::graph(index);
    if(g && g->visible())
    {
        g->setVisible(false);
        this->updateLegend();
    }
}

void QGraph::showAllGraphs()
{
    for(int i = 0; i < this->QCustomPlot::graphCount(); i++)
    {
         this->QCustomPlot::graph(i)->setVisible(true);
    }
    this->updateLegend();
}

void QGraph::hideAllGraphs()
{
    for(int i = 0; i < this->QCustomPlot::graphCount(); i++)
    {
         this->QCustomPlot::graph(i)->setVisible(false);
    }
    this->updateLegend();
}

//#include <iostream>
void QGraph::showPointToolTip(QMouseEvent * event)
{
    Q_UNUSED(event);
}
