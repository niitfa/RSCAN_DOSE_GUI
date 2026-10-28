#ifndef WEIGHTEDAVERAGE_H
#define WEIGHTEDAVERAGE_H

#include <stdint.h>

class WeightedAverage
{
    double noise = 0;
    double accumulatedWeight = 0;
public:
    WeightedAverage();
    void reset();
    void add(double data, double weight);
    double get();
};

#endif // WEIGHTEDAVERAGE_H
