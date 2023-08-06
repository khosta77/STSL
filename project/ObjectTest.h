#ifndef STEPANTESTSCRIPTLIBRARY_OBJECTTEST_H_
#define STEPANTESTSCRIPTLIBRARY_OBJECTTEST_H_

#include <string>

namespace STSL {
    /** @class ObjectTest - Сохраняем функцию в которой проходит тест в отдельную структуру, \
     *                      из которой позже произведем ее вызов
     * */
    struct ObjectTest {
        std::string name;   // Имя функции(тесты)
        double (*foo)();    // Сама функция, у нее есть своя структура. Об этом в другом месте

        ObjectTest(const std::string& n, double (*f)()) : name(n), foo(f) {}
        ~ObjectTest();

        void clear();
    };
};  // StepanTestScriptLibrary

#endif  // STEPANTESTSCRIPTLIBRARY_OBJECTTEST_H_
