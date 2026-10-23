#ifndef WIDGETADC_H
#define WIDGETADC_H

#include <QWidget>
#include <QString>
#include <QMap>
#include <map>
#include "rscan_dose_client.h"

namespace Ui {
class WidgetADC;
}

class WidgetADC : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetADC(QWidget *parent = nullptr);
    ~WidgetADC();
    void setTCPClient(RSCANDoseClient*);
    void setCommandCodes(RSCANDoseCommandCode setDR_code, RSCANDoseCommandCode setPGA_code);
    void setLabelText(QString labelPGA, QString labelDR);
    void setEnabled(bool en);
    void setDisabled(bool dis);
private slots:
    void on_comboBox_PGA_activated(const QString &arg1);

    void on_comboBox_DR_activated(const QString &arg1);

private:
    void setupComboBoxOptions();
    void setupFont();
private:
    Ui::WidgetADC *ui;
    RSCANDoseClient* client = nullptr;
    RSCANDoseCommandCode setDR_code = RSCANDoseCommandCode::Empty;
    RSCANDoseCommandCode setPGA_code = RSCANDoseCommandCode::Empty;
    QString enabledColorText;
    QString disabledColorText;
};

#endif // WIDGETADC_H
