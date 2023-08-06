#ifndef STEPANTESTSCRIPTLIBRARY_RESULTTEST_H_
#define STEPANTESTSCRIPTLIBRARY_RESULTTEST_H_

#include <string>
#include <vector>
#include <algorithm>

namespace STSL {
    /** @class ResultTest - объект для хранения и поверхностной(получение среднего) обработки тестов
     * */
    struct ResultTest {
        std::string name;               // Имя теста
        double avg;                     // Среднее значение
        double min;                     // Минимальное время
        double max;                     // Максимальнное время
        std::vector<double> values;     // Измеренные значения

        ResultTest() = default;
        ~ResultTest();

        /** @breif () - считает минимумы и максимумы, а так же среднее значение.
         * */
        void operator()();

        /** @breif clear - Метод очищает все структуры
         * */
        void clear();

    private:
        /** @breif avgv - Метод считает среднее значение вектора.
         * */
        void avgv();
    };
};  // StepanTestScriptLibrary

#endif  // STEPANTESTSCRIPTLIBRARY_RESULTTEST_H_
