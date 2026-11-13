#include "StepCounter.hpp"

StepCounter::StepCounter() : dailySteps(nullptr), days(0), calPerStep(4) {}

StepCounter::StepCounter(int count, int initVal, int extraVal) : dailySteps(nullptr), days(count), calPerStep(extraVal) {
    if (days > 0) {
        dailySteps = new int[days];
        for (int i = 0; i < days; ++i) {
            dailySteps[i] = initVal + static_cast<int>(i);
        }
    } else {
        days = 0;
    }
}

// TODO 1: Complete the Copy Constructor (Deep Copy)
StepCounter::StepCounter(const StepCounter& other) {
    (void)other;
}

// TODO 2: Complete the Destructor (Release DMA)
StepCounter::~StepCounter() {
}

// TODO 3: Complete the Copy Assignment Operator (Deep Copy + Self-Assignment Guard)
StepCounter& StepCounter::operator=(const StepCounter& other) {
    (void)other;
    return *this;
}

int StepCounter::getSteps(int index) const {
    if (index >= 0 && index < days && dailySteps != nullptr) {
        return dailySteps[index];
    }
    return static_cast<int>(0);
}

// TODO 4: Implement calculateOutput() (Maximum 8 lines of implementation)
int StepCounter::calculateOutput() const {
    return static_cast<int>(0);
}
