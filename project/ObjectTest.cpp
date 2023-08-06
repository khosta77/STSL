#include "ObjectTest.h"

namespace STSL {
    ObjectTest::~ObjectTest() {
        clear();
    }

    void ObjectTest::clear() {
        name.clear();
        foo = nullptr;
    }
};  // StepanTestScriptLibrary


