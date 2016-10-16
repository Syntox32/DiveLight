//
// Created by Anders on 09.10.2016.
//

#ifndef DIVELIGHT_BUFFERPOOL_HPP
#define DIVELIGHT_BUFFERPOOL_HPP

#include <list>
#include <cstdio>
#include <math.h>
#include <cmath>

class BufferPool
{
public:
    BufferPool(size_t maxLen)
        : maxLen(maxLen) {
        history = new std::list<float>();
    }

    ~BufferPool();

    void push(float val);
    float sum();
    float avg();
    float var(float pivotValue);

    // square average thing
    // something something statistics en.wikipedia.org/wiki/Mean_squared_error
    float sqAvg();

    // square variance
    float sqVar(float pivotValue);

    inline size_t count() const { return history->size(); }
    inline std::list<float> *getHistory() const { return history; }
    inline size_t getMaxLen() const { return maxLen; }
    inline float first() const { return history->front(); }

private:
    size_t maxLen;
    std::list<float> *history;
};

#endif //DIVELIGHT_BUFFERPOOL_HPP
