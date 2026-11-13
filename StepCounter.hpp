#ifndef STEPCOUNTER_HPP
#define STEPCOUNTER_HPP

#include <iostream>
using namespace std;

class StepCounter {
protected:
    int* dailySteps;
    int days;
    int calPerStep;

public:
    StepCounter();
    StepCounter(int count, int initVal, int extraVal);
    StepCounter(const StepCounter& other);
    virtual ~StepCounter();
    StepCounter& operator=(const StepCounter& other);

    int getSteps(int index) const;
    virtual int calculateOutput() const;
};

#endif
