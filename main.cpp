#include <iostream>
#include "StepCounter.hpp"
#include "CalorieCounter.hpp"

using namespace std;

// Lightweight heap allocation tracker using only <iostream> and C++11 primitives (no size_t, no extra headers)
static char g_heapPool[65536];
static int g_poolOffset = 0;
static int g_activeAllocs = 0;
static int g_totalAllocs = 0;

void* operator new(decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    // 8-byte alignment
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void* operator new[](decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete(void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

bool approxEqual(double a, double b) {
    double diff = a - b;
    if (diff < 0.0) diff = -diff;
    return diff < 0.001;
}

int main() {
    int passed = 0;
    int failed = 0;

    // Expected reference values computed dynamically
    // Array of 3 elements initialized as: initVal + 0, initVal + 1, initVal + 2
    int initV = static_cast<int>(1000);
    int extV = static_cast<int>(4);

    // --- PARENT CLASS TEST CASES (1 to 5) ---
    // TC 1: Default constructor & empty compute
    {
        StepCounter pDef;
        if (approxEqual(static_cast<double>(pDef.calculateOutput()), 0.0) &&
            approxEqual(static_cast<double>(pDef.getSteps(0)), 0.0)) {
            cout << "[PASS] TC 01: StepCounter default constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 01: StepCounter default constructor" << endl;
            ++failed;
        }
    }

    // TC 2: Parameterized constructor & base compute()
    {
        StepCounter p1(3, initV, extV);
        bool ok = approxEqual(static_cast<double>(p1.getSteps(0)), static_cast<double>(initV)) &&
                  approxEqual(static_cast<double>(p1.getSteps(2)), static_cast<double>(initV + static_cast<int>(2)));
        // Verify compute() is non-trivial when initV > 0
        if (ok && p1.getSteps(1) == static_cast<int>(initV + static_cast<int>(1))) {
            // Compare against a second identical instance
            StepCounter pCheck(3, initV, extV);
            if (approxEqual(static_cast<double>(p1.calculateOutput()), static_cast<double>(pCheck.calculateOutput())) &&
                !approxEqual(static_cast<double>(p1.calculateOutput()), 0.0)) {
                cout << "[PASS] TC 02: StepCounter parameterized constructor & calculateOutput()" << endl;
                ++passed;
            } else {
                cout << "[FAIL] TC 02: StepCounter calculateOutput() returned unexpected value" << endl;
                ++failed;
            }
        } else {
            cout << "[FAIL] TC 02: StepCounter element initialization mismatch" << endl;
            ++failed;
        }
    }

    // TC 3: Parent Copy Constructor (Deep Copy verification)
    {
        StepCounter orig(3, initV, extV);
        StepCounter copyObj(orig);
        if (approxEqual(static_cast<double>(copyObj.calculateOutput()), static_cast<double>(orig.calculateOutput())) &&
            approxEqual(static_cast<double>(copyObj.getSteps(1)), static_cast<double>(orig.getSteps(1))) &&
            !approxEqual(static_cast<double>(copyObj.calculateOutput()), 0.0)) {
            cout << "[PASS] TC 03: StepCounter copy constructor deep copy" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 03: StepCounter copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 4: Parent Copy Assignment Operator & Self-Assignment
    {
        StepCounter a(3, initV, extV);
        StepCounter b(2, initV, extV);
        b = a;
        StepCounter* selfPtr = &b;
        b = *selfPtr; // Self-assignment check
        if (approxEqual(static_cast<double>(b.calculateOutput()), static_cast<double>(a.calculateOutput())) &&
            approxEqual(static_cast<double>(b.getSteps(2)), static_cast<double>(a.getSteps(2))) &&
            !approxEqual(static_cast<double>(b.calculateOutput()), 0.0)) {
            cout << "[PASS] TC 04: StepCounter copy assignment & self-assignment" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 04: StepCounter copy assignment failed" << endl;
            ++failed;
        }
    }

    // TC 5: Parent Destructor Heap Release Check
    {
        int before = g_activeAllocs;
        {
            StepCounter temp(4, initV, extV);
        }
        int after = g_activeAllocs;
        if (before == after && g_totalAllocs > 0) {
            cout << "[PASS] TC 05: StepCounter destructor released heap memory" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 05: StepCounter destructor leaked heap memory" << endl;
            ++failed;
        }
    }

    // --- CHILD CLASS TEST CASES (6 to 10) ---
    // TC 6: Child Parameterized Construction & Overridden compute()
    {
        StepCounter baseObj(3, initV, extV);
        CalorieCounter childObj(3, initV, extV);
        if (!approxEqual(static_cast<double>(childObj.calculateOutput()), 0.0) &&
            !approxEqual(static_cast<double>(childObj.calculateOutput()), static_cast<double>(baseObj.calculateOutput()))) {
            cout << "[PASS] TC 06: CalorieCounter overridden calculateOutput()" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 06: CalorieCounter overridden calculateOutput() failed" << endl;
            ++failed;
        }
    }

    // TC 7: Polymorphic Dispatch via Base Pointer
    {
        CalorieCounter directChild(3, initV, extV);
        StepCounter* polyPtr = &directChild;
        if (!approxEqual(static_cast<double>(polyPtr->calculateOutput()), 0.0) &&
            approxEqual(static_cast<double>(polyPtr->calculateOutput()), static_cast<double>(directChild.calculateOutput()))) {
            cout << "[PASS] TC 07: CalorieCounter polymorphic dispatch via StepCounter*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 07: CalorieCounter polymorphic dispatch failed" << endl;
            ++failed;
        }
    }

    // TC 8: Child Copy Constructor
    {
        CalorieCounter c1(3, initV, extV);
        CalorieCounter c2(c1);
        if (approxEqual(static_cast<double>(c1.calculateOutput()), static_cast<double>(c2.calculateOutput())) &&
            !approxEqual(static_cast<double>(c2.calculateOutput()), 0.0)) {
            cout << "[PASS] TC 08: CalorieCounter copy constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 08: CalorieCounter copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 9: Child Copy Assignment Operator
    {
        CalorieCounter c1(3, initV, extV);
        CalorieCounter c2;
        c2 = c1;
        if (approxEqual(static_cast<double>(c1.calculateOutput()), static_cast<double>(c2.calculateOutput())) &&
            !approxEqual(static_cast<double>(c2.calculateOutput()), 0.0)) {
            cout << "[PASS] TC 09: CalorieCounter copy assignment operator" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 09: CalorieCounter copy assignment operator failed" << endl;
            ++failed;
        }
    }

    // TC 10: Virtual Destructor via Base Pointer Deletion
    {
        int before = g_activeAllocs;
        StepCounter* heapChild = new CalorieCounter(4, initV, extV);
        delete heapChild;
        int after = g_activeAllocs;
        if (before == after) {
            cout << "[PASS] TC 10: CalorieCounter virtual destructor via StepCounter*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 10: CalorieCounter virtual destructor leaked memory" << endl;
            ++failed;
        }
    }

    cout << "========================================" << endl;
    cout << "FINAL SCORE: " << passed << " / 10 PASSED (" << failed << " FAILED)" << endl;
    if (g_activeAllocs == 0) {
        cout << "MEMORY LEAK REPORT: NO MEMORY LEAKS DETECTED (0 active allocations)." << endl;
    } else {
        cout << "MEMORY LEAK REPORT: WARNING! " << g_activeAllocs << " active heap allocation(s) leaked!" << endl;
    }
    cout << "========================================" << endl;

    return (failed == 0 && g_activeAllocs == 0) ? 0 : 1;
}
