#ifndef STEPANTESTSCRIPTLIBRARY_TIMETEST_H_
#define STEPANTESTSCRIPTLIBRARY_TIMETEST_H_

#include <exception>
#include <iomanip>

#include "DataFrame.h"
#include "GetCPUTime.h"

namespace STSL {
    class _no_some_data_in_add : public std::exception {
    public:
        _no_some_data_in_add() = default;
        virtual void print() noexcept {    std::cout << "Не указан какой-то тип данных" << std::endl; }
    };

    /** @class TimeTest - класс, который заполняется тестами и их выполняет, возращая набор данных
     */
    class TimeTest {
        std::string CPU;
        ResultDF rdf;
        ObjectDF odf;
        const size_t Test_Count = 100;
        
        // 'Макросы' для проверки getCPUTime();
        const std::string Group_getCPUTime = "Проверка скорости внутренних функций, нужных для уточнения погрешности";
        const std::string Name_getCPUTime = "Проверка скорости работы функции getCPUTime()";

        // 'Макросы' для добавления переменных
        const int Object_Test_Add = 0;
        const int Group_No_Existing = -1;

        const size_t Bar_Width = 60;
        const char Symbol_Bar = '=';

        size_t moment_group = 0;
        size_t group_cout_size = 0;

        void printGroup();
        int progress_bar(const double &progress);
        std::vector<double> make_timetest(const ObjectTest &ot) noexcept;
        void make_group_tests(std::pair<std::string, std::vector<ResultTest>> &brdf, const std::vector<ObjectTest> &ots);
        void make_groups_tests();
        ResultDF MakeTest();

        /** @brief put_getCPUTime - Приписывается в конструкторе, вызывается для того, чтобы провести \
         *                          проверку метода который может давать погрешность на результат.
         * */
        void put_getCPUTime();

        /** @brief push_object_test_in_existing_group - Метод в УЖЕ существующую группы добавляем объект \
         *                                             тестов(имя и указатель на тестируемую функцию)
         * */
        int push_object_test_in_existing_group(const std::string &gp, const std::string &tn, double (*f)());

        /** @brief push_new_group - Метод добавляет новую группу
         * */
        void push_new_group(const std::string &gp, const std::string &tn, double (*f)());

    public:
        TimeTest(const std::string &CPUname) : CPU(CPUname) {   put_getCPUTime(); }
        ~TimeTest();

        /** @brief add - Метод для добавления тестов.
         *  @param gp - Имя группы, в группа предполагается идейное объединение тестов
         *  @param tn - testname - имя теста
         *  @param f - функция которую тестируют на время
         * */
        void add(const std::string &gp, const std::string &tn, double (*f)());
        
        /** @brief dotests - Метод проводит тест и возращает пару(Процессор и результат)
         * */
        std::pair<std::string, ResultDF> dotests();

        /** @brief clear - Метод проводит очистку типов данных
         * */
        void clear();
    };
};  // StepanTestScriptLibrary

#endif // STEPANTESTSCRIPTLIBRARY_TIMETEST_H_
