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

    // override
    void setEnabled(bool);
    void setDisabled(bool);
private:
    void drawFrame();
    void setupFont();
private:
    Ui::WidgetText *ui;
    QFont font;
    QString enabledColorText;
    QString disabledColorText;
};

#endif // WIDGETTEXT_H
