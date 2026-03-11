#ifndef WIDGETTEXT_H
#define WIDGETTEXT_H

#include <QWidget>
#include <QFont>

namespace Ui {
class WidgetText;
}

class WidgetText : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetText(QWidget *parent = nullptr);
    ~WidgetText();
    void setText(const QString&);
private:
    void drawFrame();
    void setupFont();
private:
    Ui::WidgetText *ui;
    QFont font;
};

#endif // WIDGETTEXT_H
