#ifndef SCANSESSIONFILE_H
#define SCANSESSIONFILE_H

#include <QVector>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QDate>
#include <QVector>

class Logger
{
    QString filepath;
    QFile* file = nullptr;

    // special - regular
    uint32_t  id = 0;
    int signalA = 0;
    int signalB = 0;
    int voltageA = 0;
    int voltageB = 0;
    int polarityA = 0;
    int polarityB = 0;
    int sensitivityA = 0;
    int sensitivityB = 0;
    int hvEnabledA = 0;
    int hvEnabledB = 0;
    int temperature = 0;

    // other
    QString filename = "rscan_dose";
public:
    Logger();
    ~Logger();

    bool start();
    void update();
    void stop();

    void setFilename(QString);

    // regular
    void setID(uint32_t id);
    void setSignals(int A, int B);
    void setVoltages(int A, int B);
    void setPolarities(int A, int B);
    void setSensitivities(int A, int B);
    void setHVEnabled(int A, int B);
    void setTemperature(int temp);

private:
    QString getFolderPath();
    QString createCurrentTimeStr();
    QString QStringFromInt(int num, int numMinimumLength);
    void printHeadText(QFile* f);
    void printValuesDescription(QFile* f);
    void printReqularData(QFile* f);
    QString getTimeStringFile();
    QString getDateStringFile();

};

#endif // SCANSESSIONFILE_H
