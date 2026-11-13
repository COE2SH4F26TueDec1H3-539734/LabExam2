#ifndef CALORIECOUNTER_HPP
#define CALORIECOUNTER_HPP

#include "StepCounter.hpp"

class CalorieCounter : public StepCounter {
public:
    CalorieCounter();
    CalorieCounter(int count, int initVal, int extraVal);
    CalorieCounter(const CalorieCounter& other);
    virtual ~CalorieCounter();
    CalorieCounter& operator=(const CalorieCounter& other);

    virtual int calculateOutput() const override;
};

#endif
