#include "CalorieCounter.hpp"

CalorieCounter::CalorieCounter() : StepCounter() {}

CalorieCounter::CalorieCounter(int count, int initVal, int extraVal) : StepCounter(count, initVal, extraVal) {}

CalorieCounter::CalorieCounter(const CalorieCounter& other) : StepCounter(other) {}

CalorieCounter::~CalorieCounter() {}

CalorieCounter& CalorieCounter::operator=(const CalorieCounter& other) {
    if (this != &other) {
        StepCounter::operator=(other);
    }
    return *this;
}

// TODO 5: Override calculateOutput() (Maximum 8 lines of implementation)
int CalorieCounter::calculateOutput() const {
    return static_cast<int>(0);
}
