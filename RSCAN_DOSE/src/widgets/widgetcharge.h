#ifndef WIDGETCHARGE_H
#define WIDGETCHARGE_H

#include <QWidget>
#include "rscan_dose_client.h"

namespace Ui {
class WidgetCharge;
}

class WidgetCharge : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetCharge(QWidget *parent = nullptr);
    ~WidgetCharge();
     void setClient(RSCANDoseClient*);
     void init();
private:
    Ui::WidgetCharge *ui;
    RSCANDoseClient* client = nullptr;
};

#endif // WIDGETCHARGE_H
