#ifndef WIDGETACTIVITY_H
#define WIDGETACTIVITY_H

#include <QWidget>
#include <QFont>

// add tcp client class

namespace Ui {
class WidgetActivity;
}

class WidgetActivity : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetActivity(QWidget *parent = nullptr);
    ~WidgetActivity();

private slots:
    void on_pushButton_startMeasurement_clicked();
    void on_pushButton_resetMeasurement_clicked();

private:
    Ui::WidgetActivity *ui;
    void drawStartStateButton();
    void drawStopStateButton();
    void drawCurrentStartButtonState();
private:
    bool buttonInStartState = 1;
    QFont buttonsFont;
};

#endif // WIDGETACTIVITY_H
