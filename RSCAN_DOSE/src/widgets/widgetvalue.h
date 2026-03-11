#ifndef WIDGETVALUE_H
#define WIDGETVALUE_H

#include <QWidget>
#include <QString>
#include "qgraph.h"

namespace Ui {
class WidgetValue;
}

class WidgetValue : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetValue(QWidget *parent = nullptr);
    ~WidgetValue();

    void setHeadText(QString qstr);
    void setValueText(QString qstr);

    // override
    void setEnabled(bool);
    void setDisabled(bool);

private:
    Ui::WidgetValue *ui;

    QGraph* qgraph = nullptr;
    int graphIndex = 0;
    static const int lineLength = 4;
    QString enabledColorText;
    QString disabledColorText;
private:
};

#endif // WIDGETVALUE_H
