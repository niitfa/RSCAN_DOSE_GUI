#ifndef WIDGETGRAPH_H
#define WIDGETGRAPH_H

#include <QWidget>
#include <QPushButton>
#include <QFont>
#include <QVector>
#include "qgraph.h"

namespace Ui {
class WidgetGraph;
}

class WidgetGraph : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetGraph(QWidget *parent = nullptr);
    ~WidgetGraph();

    // override
    void setEnabled(bool);
    void setDisabled(bool);

    // graph management
    void setLineVisible(int lineNo, bool visible);
    void pushData(const QVector<double> &);

private slots:
    void on_pushButton_startGraph_clicked();

    void on_lineEdit_graphMin_editingFinished();

    void on_lineEdit_graphMax_editingFinished();

    void on_lineEdit_time_editingFinished();
private:
    // construstor wrappers
    void setupStartButton();
    void setupFont();
    void setupTexts();
    void setupGraph();
    void setupLineEdits();

    void setStartStyle(QPushButton * button, QString text);
    void setStopStyle(QPushButton * button, QString text);
    void setEnabledTextColor(bool);

    void startGraph();
    void stopGraph();

    void resizeEvent(QResizeEvent *) override;
private:
    Ui::WidgetGraph *ui;
    QString graphButtonStartText;
    QString graphButtonStopText;
    QString enabledColorText;
    QString disabledColorText;
    QFont buttonsFont;

    // graph
    QGraph* graph = nullptr;
    bool graphStarted = true;
    double yGraphMaxRange = 9e+6; // was 9000000
    double yGraphMinRange = -1e+6;
    double tGraphRange = 20;
};

#endif // WIDGETGRAPH_H
