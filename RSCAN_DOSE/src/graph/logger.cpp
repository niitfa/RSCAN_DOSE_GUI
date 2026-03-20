#include "logger.h"
#include <QDate>
#include <QTime>
#include <QtMath>
#include <QTextStream>
#include <iostream>

Logger::Logger()
{
}

Logger::~Logger()
{
}

bool Logger::start()
{
    // create filepath
    this->filepath = this->getFolderPath();

    // open file, check if opened
    QString fname = this->filename;

    // check dir existing
    QDir dir;

    if(!dir.exists(this->filepath))
        dir.mkpath(this->filepath);

    file = new QFile(this->filepath + fname);
    if(file->open(QIODevice::ReadWrite))
    {
        printHeadText(file);
        printValuesDescription(this->file);
        return true;
    }
    return false;
}

void Logger::update()
{
    if(this->file)
    {
        printReqularData(this->file);
    }
}

void Logger::stop()
{
    // clear
    this->file->close();
    delete this->file;
    this->file = nullptr;
}

void Logger::setFilename(QString filename)
{
    this->filename = filename;
}

void Logger::setID(uint32_t id)
{
    this->id = id;
}

void Logger::setSignals(int A, int B)
{
    signalA = A;
    signalB = B;
}

void Logger::setVoltages(int A, int B)
{
    voltageA = A;
    voltageB = B;
}

void Logger::setPolarities(int A, int B)
{
    polarityA = A;
    polarityB = B;
}

void Logger::setSensitivities(int A, int B)
{
    sensitivityA = A;
    sensitivityB = B;
}

void Logger::setHVEnabled(int A, int B)
{
    hvEnabledA = A;
    hvEnabledB = B;
}

void Logger::setTemperature(int temp)
{
    temperature = temp;
}

// private
QString Logger::getFolderPath()
{
    QString path = QDir::currentPath();
    path += "/RSCAN_DOSE_SESSIONS/" + this->createCurrentTimeStr() + "_" + filename + "/";
    return path;
}

QString Logger::createCurrentTimeStr()
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    QString str;
    str += this->QStringFromInt(date.year(), 4);
    str += "-";
    str += this->QStringFromInt(date.month(), 2);
    str += "-";
    str += this->QStringFromInt(date.day(), 2);
    str += "-";
    str += this->QStringFromInt(time.hour(), 2);
    str += "-";
    str += this->QStringFromInt(time.minute(), 2);
    str += "-";
    str += this->QStringFromInt(time.second(), 2);
    return str;
}

QString Logger::QStringFromInt(int num, int numMinimumLength)
{
    if(numMinimumLength)
    {
        int numCopy = num;
        int numLength = 1;
        while((numCopy /= 10)) { numLength++; }
        numLength = qMax(numLength, numMinimumLength);

        QString res;
        int div = static_cast<int>(qPow(10, numLength - 1));
        for(int i = 0; i < numLength; i++)
        {
            int digit = num / div;
            res += QString::number(digit);
            num %= div;
            div /= 10;
        }
        return res;
    }
    return QString::number(num);
}

void Logger::printHeadText(QFile* f)
{
    QTextStream stream(f);
    stream << "Date," <<  getDateStringFile() << endl;
    stream << "Time," <<  getTimeStringFile() << endl;
    stream << endl;
    stream << "Channel," << "A," << "B," << endl;
    stream << "Enabled," << (hvEnabledA ? "Yes," : "No,") << (hvEnabledB ? "Yes," : "No,")<< endl;
    stream << "Voltage (V)," << voltageA << "," <<  voltageB << "," << endl;
    stream << "Polarity," << (polarityA ? "+," : "-,") << (polarityB ? "+," : "-,") << endl;
    stream << "Sensitivity," << (sensitivityA ? "High," : "Low,") << (sensitivityB ? "High," : "Low,") << endl;
    stream << endl;

}

void Logger::printValuesDescription(QFile *f)
{
    QTextStream stream(f);
    stream << "Time," << "Date," << "ID," << "Enabled A," << "Signal A," << "Voltage A (V)," << "Polarity A," << "Sensitivity A,"
                                          << "Enabled B," << "Signal B," << "Voltage B (V)," << "Polarity B," << "Sensitivity B,"
                                          << "Temperature (℃)";
    stream << endl;
}

void Logger::printReqularData(QFile *f)
{
    QTextStream stream(f);

    // заполнение строки
    stream <<
              // common
              getTimeStringFile() << "," <<
              getDateStringFile() << "," <<
              QString::number(id) << "," <<
              // channel A
              (hvEnabledA ? "Yes," : "No,") <<
              signalA << "," <<
              voltageA << "," <<
              (polarityA ? "+," : "-,") <<
              (sensitivityA ? "High," : "Low,") <<
              (hvEnabledB ? "Yes," : "No,") <<
              // channel B
              signalB << "," <<
              voltageB << "," <<
              (polarityB ? "+," : "-,") <<
              (sensitivityB ? "High," : "Low,") <<

              temperature << "," << endl;
}

QString Logger::getTimeStringFile()
{
    QTime time = QTime::currentTime();

    QString str;
    str += this->QStringFromInt(time.hour(), 2);
    str += ":";
    str += this->QStringFromInt(time.minute(), 2);
    str += ":";
    str += this->QStringFromInt(time.second(), 2);
    str += ".";
    str += this->QStringFromInt(time.msec(), 3);
    return str;
}

QString Logger::getDateStringFile()
{
    QDate date = QDate::currentDate();

    QString str;
    str += this->QStringFromInt(date.day(), 2);
    str += ".";
    str += this->QStringFromInt(date.month(), 2);
    str += ".";
    str += this->QStringFromInt(date.year(), 4);
    return str;
}
