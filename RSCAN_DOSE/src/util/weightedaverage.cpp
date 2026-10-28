#include "weightedaverage.h"

WeightedAverage::WeightedAverage()
{
    reset();
}

void WeightedAverage::reset()
{
    noise = 0;
    accumulatedWeight = 0;
}

void WeightedAverage::add(double data, double weight)
{
    accumulatedWeight += weight;
    noise = (accumulatedWeight - weight) / accumulatedWeight * noise;
    noise += weight / accumulatedWeight * data;
}

double WeightedAverage::get()
{
    return noise;
}
