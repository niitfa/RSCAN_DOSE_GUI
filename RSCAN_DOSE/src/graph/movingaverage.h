#ifndef MOVINGAVERAGE_H
#define MOVINGAVERAGE_H

#include <vector>
#include <QVector>

class MovingAverage
{
public:
    MovingAverage();
    void addElement(double);
    double getAverage();
    void setAveragingLevel(uint16_t averagingLevel);
    uint16_t getAveragingLevel();
    static const uint16_t maxSize = 500;
private:
    QVector<double> data = QVector<double>(maxSize);
private:
    uint16_t averagingLevel = 1;
};

#endif // MOVINGAVERAGE_H
