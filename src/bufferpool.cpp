//
// Created by Anders on 09.10.2016.
//

#include "../include/bufferpool.hpp"

BufferPool::~BufferPool()
{
    delete history;
}

void BufferPool::push(float val)
{
    if (history->size() == maxLen)
        history->pop_back();

    history->push_front(val);
}

float BufferPool::sum()
{
    float ret = 0.0f;

    for (auto& val : *history)
        ret += val;

    return ret;
}

float BufferPool::avg()
{
    float sumVal = sum();
    return (float)(sumVal / history->size());
}

float BufferPool::var(float pivotValue)
{
    float ret = 0.0f;

    for (auto& val : *history)
        ret += val - pivotValue;

    return (ret / (float)history->size());
}

// squared average thing
// something something statistics en.wikipedia.org/wiki/Mean_squared_error
float BufferPool::sqAvg()
{
    float ret = 0.0f;

    for (auto& val : *history)
        ret += (val * val);

    return ((ret) / (float)history->size());
}

// squared variance
float BufferPool::sqVar(float pivotValue)
{
    float ret = 0.0f;

    for (auto& val : *history)
        ret += (float)pow(val - pivotValue, 2);

    return (ret / (float)history->size());
}