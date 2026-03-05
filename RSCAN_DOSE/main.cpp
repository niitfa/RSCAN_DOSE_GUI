#include "mainwindow.h"
#include <QApplication>

/* Settings includes */
#include <QtCore/QCoreApplication>
#include <QUrl>
#include <QFile>
#include <QString>
#include "settings.h"

void setupConfig();

int main(int argc, char *argv[])
{
    setupConfig();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


void setupConfig()
{
    QCoreApplication::setOrganizationName("RT7");
    QCoreApplication::setApplicationName("RSCAN DOSE");
    QFile cfgDefaults(":/config/default.cfg");
    cfgDefaults.open(QIODevice::ReadOnly);
    Settings::setDefaults(cfgDefaults.readAll());
}
