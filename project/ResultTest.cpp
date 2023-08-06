#include "ResultTest.h"

namespace STSL {
    ResultTest::~ResultTest() {
        clear();
    }

    void ResultTest::operator()() {
        min = *std::min_element(values.begin(), values.end());
        max = *std::max_element(values.begin(), values.end());
        avgv();
    }

    void ResultTest::clear() {
        name.clear();
        values.clear();
    }

    void ResultTest::avgv() {
        avg = values[0];
        for (size_t i = 1; i < values.size(); ++i)
            avg += values[i];
        avg /= values.size();
    }
};  // StepanTestScriptLibrary


