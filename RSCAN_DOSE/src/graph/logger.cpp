#include "logger.h"
#include <QDate>
#include <QTime>
#include <QtMath>
#include <QTextStream>
#include <iostream>
#include "rscan_dose_client.h"
#include "settings.h"

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

void Logger::setCurrents(double A, double B)
{
    currentA = A;
    currentB = B;
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
    // sensitivity A
    QString sensitivityA_str;
    if(sensitivityA == static_cast<uint8_t>(RSCANDoseSensitivity::Low)) sensitivityA_str = "Low,";
    if(sensitivityA == static_cast<uint8_t>(RSCANDoseSensitivity::High)) sensitivityA_str = "High,";
    // sensitivity B
    QString sensitivityB_str;
    if(sensitivityB == static_cast<uint8_t>(RSCANDoseSensitivity::Low)) sensitivityB_str = "Low,";
    if(sensitivityB == static_cast<uint8_t>(RSCANDoseSensitivity::High)) sensitivityB_str = "High,";

    QTextStream stream(f);
    stream << "Date," <<  getDateStringFile() << endl;
    stream << "Time," <<  getTimeStringFile() << endl;
    stream << endl;
    stream << "Channel," << "A," << "B," << endl;
    stream << "Enabled," << (hvEnabledA ? "Yes," : "No,") << (hvEnabledB ? "Yes," : "No,")<< endl;
    stream << "Voltage (V)," << voltageA << "," <<  voltageB << "," << endl;
    stream << "Polarity," << (polarityA ? "+," : "-,") << (polarityB ? "+," : "-,") << endl;
    stream << "Sensitivity," << sensitivityA_str << sensitivityB_str << endl;
    stream << "k Low (A/count),"
           << Settings::get(Settings::Key::K_A_Low, Settings::Section::Main).toString() + ","
           << Settings::get(Settings::Key::K_B_Low, Settings::Section::Main).toString() + ","
           << endl;
    stream << "k High (A/count),"
           << Settings::get(Settings::Key::K_A_High, Settings::Section::Main).toString() + ","
           << Settings::get(Settings::Key::K_B_High, Settings::Section::Main).toString() + ","
           << endl;
    stream << endl;

}

void Logger::printValuesDescription(QFile *f)
{
    QTextStream stream(f);
    stream << "Time," << "Date," << "ID," << "Enabled A," << "Signal A," << "Current A (pA)," << "Voltage A (V)," << "Polarity A," << "Sensitivity A,"
                                          << "Enabled B," << "Signal B," << "Current A (pA)," << "Voltage B (V)," << "Polarity B," << "Sensitivity B,"
                                          << "Temperature (℃)";
    stream << endl;
}

#include <iostream>
void Logger::printReqularData(QFile *f)
{
    QTextStream stream(f);

    // sensitivity A
    QString sensitivityA_str;
    if(sensitivityA == static_cast<uint8_t>(RSCANDoseSensitivity::Low)) sensitivityA_str = "Low,";
    if(sensitivityA == static_cast<uint8_t>(RSCANDoseSensitivity::High)) sensitivityA_str = "High,";
    // sensitivity B
    QString sensitivityB_str;
    if(sensitivityB == static_cast<uint8_t>(RSCANDoseSensitivity::Low)) sensitivityB_str = "Low,";
    if(sensitivityB == static_cast<uint8_t>(RSCANDoseSensitivity::High)) sensitivityB_str = "High,";

    // заполнение строки
    stream <<
              // common
              getTimeStringFile() << "," <<
              getDateStringFile() << "," <<
              QString::number(id) << "," <<
              // channel A
              (hvEnabledA ? "Yes," : "No,") <<
              signalA << "," <<
              currentA << "," <<
              voltageA << "," <<
              (polarityA ? "+," : "-,") <<
              sensitivityA_str <<
              (hvEnabledB ? "Yes," : "No,") <<
              // channel B
              signalB << "," <<
              currentB << "," <<
              voltageB << "," <<
              (polarityB ? "+," : "-,") <<
              sensitivityB_str <<
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
