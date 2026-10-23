#include "movingaverage.h"

#include <iostream>

MovingAverage::MovingAverage()
{ 
    averagingLevel = 1;
}

void MovingAverage::addElement(double val)
{
    data.pop_front();
    data.push_back(val);
}

void MovingAverage::setAveragingLevel(uint16_t averagingLevel)
{
    this->averagingLevel = (averagingLevel > maxSize) ? maxSize : averagingLevel;
}

uint16_t MovingAverage::getAveragingLevel()
{
    return averagingLevel;
}

double MovingAverage::getAverage()
{
    double ret = 0;
    if(averagingLevel >= 0)
    {
        for(int i = data.size() - 1; i >= data.size() - averagingLevel; i--)
        {
             ret += data[i] / averagingLevel;
        }
    }
    else
    {
        ret = data.back();
    }
    return ret;
}


