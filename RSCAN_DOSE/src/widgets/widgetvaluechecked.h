#ifndef WIDGETVALUECHECKED_H
#define WIDGETVALUECHECKED_H

#include <QWidget>
#include <functional>

namespace Ui {
class WidgetValueChecked;
}

class WidgetValueChecked : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetValueChecked(QWidget *parent = nullptr);
    ~WidgetValueChecked();

    void setHeadText(QString qstr);
    void setValueText(QString qstr);
    void setCheckedCallback(std::function<void(bool)> cb);
    void setChecked(bool);

    // override
    void setEnabled(bool);
    void setDisabled(bool);

private slots:
    void on_checkBox_clicked();

private:
    Ui::WidgetValueChecked *ui;
    QString enabledColorText;
    QString disabledColorText;
    std::function<void(bool)> checkedCallback = [](bool){};

};

#endif // WIDGETVALUECHECKED_H
