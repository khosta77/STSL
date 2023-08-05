#ifndef STEPANTESTSCRIPTLIBRARY_TIMETEST_H_
#define STEPANTESTSCRIPTLIBRARY_TIMETEST_H_



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#if defined(_WIN32)  // автор метода ниже: https://habr.com/ru/post/282301/
#include <Windows.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>
#else
#error "Unable to define getCPUTime() for an unknown OS."
#endif


namespace STSL {
// ResultTest.h
    /** @class ResultTest - объект для хранения и поверхностной(получение среднего) обработки тестов
     * */
    struct ResultTest {
        std::string name;               // Имя теста
        double avg;                     // Среднее значение
        double min;                     // Минимальное время
        double max;                     // Максимальнное время
        std::vector<double> values;     // Измеренные значения

        ResultTest() = default;
 
        ~ResultTest() {
            clear();
        }
        
        /** @breif () - считает минимумы и максимумы, а так же среднее значение.
         * */
        void operator()() {
            min = *std::min_element(values.begin(), values.end());
            max = *std::max_element(values.begin(), values.end());
            avgv();
        }

        /** @breif clear - Метод очищает все структуры
         * */
        void clear() {
            name.clear();
            values.clear();
        }

    private:
        /** @breif avgv - Метод считает среднее значение вектора.
         * */
        void avgv() {
            avg = values[0];
            for (size_t i = 1; i < values.size(); ++i)
                avg += values[i];
            avg /= values.size();
        }
    }; 

// ObjectTest.h
    /** @class ObjectTest - Сохраняем функцию в которой проходит тест в отдельную структуру, \
     *                      из которой позже произведем ее вызов
     * */
    struct ObjectTest {
        std::string name;   // Имя функции(тесты)
        double (*foo)();    // Сама функция, у нее есть своя структура. Об этом в другом месте

        ObjectTest(const std::string& n, double (*f)()) : name(n), foo(f) {} 

        ~ObjectTest() {
            clear();
        }

        void clear() {
            name.clear();
            delete foo;
        }
    };

// GetCPUTime.h
    /** @breif getCPUTime - \
     * Возвращает количество процессорного времени, используемого текущим процессом, \
     * в секундах или -1,0, если произошла ошибка. \
     * Returns the amount of CPU time used by the current process, \
     * in seconds, or -1.0 if an error occurred.
     */
    double getCPUTime() {
#if defined(_WIN32)
        /* Windows -------------------------------------------------- */
        FILETIME createTime;
        FILETIME exitTime;
        FILETIME kernelTime;
        FILETIME userTime;
        if ( GetProcessTimes( GetCurrentProcess( ), &createTime, &exitTime, &kernelTime, &userTime ) != -1 ) {
            SYSTEMTIME userSystemTime;
            if ( FileTimeToSystemTime( &userTime, &userSystemTime ) != -1 )
                return (double)userSystemTime.wHour * 3600.0 +
                       (double)userSystemTime.wMinute * 60.0 +
                       (double)userSystemTime.wSecond +
                       (double)userSystemTime.wMilliseconds / 1000.0;
        }

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
        /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */

#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
        /* Prefer high-res POSIX timers, when available. */
        {
            clockid_t id;
            struct timespec ts;
#if _POSIX_CPUTIME > 0
            /* Clock ids vary by OS.  Query the id, if possible. */
            if ( clock_getcpuclockid( 0, &id ) == -1 )
#endif
#if defined(CLOCK_PROCESS_CPUTIME_ID)
                /* Use known clock id for AIX, Linux, or Solaris. */
                id = CLOCK_PROCESS_CPUTIME_ID;
#elif defined(CLOCK_VIRTUAL)
            /* Use known clock id for BSD or HP-UX. */
            id = CLOCK_VIRTUAL;
#else
            id = (clockid_t)-1;
#endif
            if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
                return (double)ts.tv_sec +
                       (double)ts.tv_nsec / 1000000000.0;
        }
#endif

#if defined(RUSAGE_SELF)
        {
            struct rusage rusage;
            if ( getrusage( RUSAGE_SELF, &rusage ) != -1 )
                return (double)rusage.ru_utime.tv_sec +
                       (double)rusage.ru_utime.tv_usec / 1000000.0;
        }
#endif

#if defined(_SC_CLK_TCK)
        {
            const double ticks = (double)sysconf( _SC_CLK_TCK );
            struct tms tms;
            if ( times( &tms ) != (clock_t)-1 )
                return (double)tms.tms_utime / ticks;
        }
#endif

#if defined(CLOCKS_PER_SEC)
        {
            clock_t cl = clock( );
            if ( cl != (clock_t)-1 )
                return (double)cl / (double)CLOCKS_PER_SEC;
        }
#endif

#endif
        return -1;
    }

    // Сокращения работы с сложными объектами
    //                                      Группа       frame группы
    using ObjectDF = std::vector<std::pair<std::string, std::vector<ObjectTest>>>;  // ObjectDataFrame
    using ResultDF = std::vector<std::pair<std::string, std::vector<ResultTest>>>;  // ResultDataFrame

    //                                      Процессор    Результат на нем
    using ResultsDF = std::vector<std::pair<std::string, ResultDF>>  // ResultsDataFrameOnProcessor

    using Test_Count = 10;  // 'Макрос' колличества проводимых тестов

    namespace DataFrameFunction {
        void clear_buffer_pair_rt(std::pair<std::string, std::vector<ResultTest>>> &df) {
            df.first.clear();
            for (size_t i = 0, N = df.second.size(); i < N; ++i) {
                rdf.second[i].clear();
            }
            df.second.clear();
        }

        void clear_result_data_frame(ResultDF &rdf) {
            for (size_t i = 0, N = rdf.size(); i < N; ++i) {
                rdf[i].first.clear();
                for (size_t j = 0, M = rdf[i].second.size(); j < M; ++j)
                    rdf[i].second[j].clear();
                rdf[i].second.clear();
            }
        }

        void clear_object_data_frame(ObjectDF &odf) {
            for (size_t i = 0, N = odf.size(); i < N; ++i) {
                odf[i].first.clear();
                for (size_t j = 0, M = odf[i].second.size(); j < M; ++j) {
                    odf[i].second[j].clear();
                }
                odf[i].second.clear();
            }
        }

        void clear_results_data_frame(ResultsDF &rsdf) {
            for (size_t i = 0, N = rsdf.size(); i < N; ++i) {
                rsdf[i].first.clear();
                clear_result_data_frame(rsdf[i].second);
            }
        }

        void print_pair_resultdf(const std::pair<std::string, ResultDF> &prdf) {
            std::cout << "CPU: " << prdf.first << std::endl;
            for (size_t i = 0, N = prdf.second.size(); i < N; ++i) {
                std::cout << prdf.second[i].first << std::endl;
                for (size_t j = 0, M = prdf.second[i].second.size(); j < M; ++j) {
                    std::cout << prdf.second[i].second[j].name << "\r\n\t"
                              << prdf.second[i].second[j].avg << std::endl;
                }
            }
        }
    };

// TimeTest.h
    class _no_some_data_in_add : public std::exception {
    public:
        _no_group_test() = default;

        virtual void print() noexcept {
            std::cout << "Не указан какой-то тип данных" << std::endl;
        }
    };

    /** @class TimeTest - класс, который заполняется тестами и их выполняет, возращая набор данных
     */
    class TimeTest {
        std::string CPU;
        ResultDF rdf;
        ObjectDF odf;
        const size_t test_count = Test_Count;
        
        // 'Макросы' для проверки getCPUTime();
        using Group_getCPUTime = std::string("Группа: Проверка скорости внутренних функций, нужных для уточнения погрешности");
        using Name_getCPUTime = std::string("Проверка скорости работы функции getCPUTime()");

        // 'Макросы' для добавления переменных
        using Object_Test_Add = 0;
        using Group_No_Existing = -1;

        std::vector<double> make_timetest(const ObjectTest &ot) noexcept {
            double startTime, endTime, inaccuracy;
            std::vector<double> timeframe;
            for (size_t cnt = 0; cnt < test_count; ++cnt) {
                startTime = getCPUTime();
                inaccuracy = ot.foo();
                endTime = getCPUTime();
                timeframe.push_back((endTime - startTime - inaccuracy));
            }
            return timeframe;
        }

        void make_group_tests(std::pair<std::string, std::vector<ResultTest>>> &brdf, const std::vector<ObjectTest> &ots) {
            ResultTest brt;
            for (size_t i = 0, N = ots.size(); i < N; ++i) {
                brt.name = ots[i].name;
                brt.values = make_timetest(ots[i]);
                brt();
                brdf.second.push_back(brt);
            }
            brt.clear();
        }

        void make_groups_tests() {
            std::pair<std::string, std::vector<ResultTest>>> brdf;
            for (size_t i = 0, N = odf.size(); i < N; ++i) {
                brdf.first = odf[i].first;
                make_group_tests(brdf, odf[i].second);
                rdf.push_back(brdf);
                DataFrameFunction::clear_buffer_pair_rt(brdf);
            }
        }

        ResultDF MakeTest() {
            make_groups_tests(brdf);
            return rdf;
        }

        /** @brief put_getCPUTime - Приписывается в конструкторе, вызывается для того, чтобы провести \
         *                          проверку метода который может давать погрешность на результат.
         * */
        void put_getCPUTime() {
            push_new_group(Group_getCPUTime, Name_getCPUTime, getCPUTime);
        }

        /** @brief push_object_test_in_existing_group - Метод в УЖЕ существующую группы добавляем объект \
         *                                             тестов(имя и указатель на тестируемую функцию)
         * */
        const int push_object_test_in_existing_group(const std::string &gp, const std::string &tn, double (*f)()) {
            for (size_t i = 0, N = odf.size(); i < N; ++i) {
                if (odf[i].first == gp) {
                    odf[i].second.push_back(ObjectTest(tn, f));
                    return Object_Test_Add;
                }
            }
            return Group_No_Existing;
        }

        /** @brief push_new_group - Метод добавляет новую группу
         * */
        void push_new_group(const std::string &gp, const std::string &tn, double (*f)()) {
            std::pair<std::string, std::vector<ResultTest>> bdf;
            bdf.first = gp;
            bdf.second.push_back(ObjectTest(tn, f));
            odf.push_back(bdf);
        }

    public:
        TimeTest(const std::string &CPUname) : CPU(CPUname) {
            put_getCPUTime();
        }

        ~TimeTest() {
            clear();
        }

        /** @brief add - Метод для добавления тестов.
         *  @param gp - Имя группы, в группа предполагается идейное объединение тестов
         *  @param tn - testname - имя теста
         *  @param f - функция которую тестируют на время
         * */
        void add(const std::string &gp, const std::string &tn, double (*f)()) {
            if (gp.empty() || tn.empty() || (f == nullptr))
                throw _no_some_data_in_add(); 
            if (push_object_test_in_existing_group(gp, tn, f) == Group_No_Existing)
                push_new_group(gp, tn, f);
        }
        
        /** @brief dotests - Метод проводит тест и возращает пару(Процессор и результат)
         * */
        std::pair<std::string, ResultDF> dotests() {
            return std::pair<std::string, ResultDF>(CPUname, MakeTest());
        }

        /** @brief clear - Метод проводит очистку типов данных
         * */
        void clear() {
            CPUname.clear();
            DataFrameFunction::clear_result_data_frame(rdf);
            DataFrameFunction::clear_object_data_frame(odf);
        }
    };

// ResultOut.h
    class ResultsOut {
        ResultsDF rsdf;  // Массив результатов
       
        void TexResults();

        using Coloumn_Count = rsdf.size();

        /** @brief printTableTitle - Вывод объявление таблицы формата tex
         * */
        void printTableTitle() {
            std::cout << "\\begin{tabular}{|p{2.0in}|p{1.5in}|";
            for (size_t i = 0; i < Coloumn_Count; ++i) {
                std::cout << "p{1.5in}|";
            }
            std::cout << "}\n";
            std::cout << "\\hline\n";
        }

        /** @brief printTableHead -  Вывод шапки таблицы 
         * */
        void printTableHead() {
            std::cout << "Название & Времнная & \\multicolumn{" << Coloumn_Count << "}{c|}{Процессор}  \\\\ \\cline{3-" << (2 + Coloumn_Count) << "}\n";
            std::cout << "теста    & характеристика & ";
            for (size_t i = 0; i < (Coloumn_Count - 1); ++i) {
                std::cout << rsdf[i].first << " & ";
            }
            std::cout << rsdf.back().first << " \\\\  \\hline\n"; 
        }

        void printGroupTitle(const std::string &gn) {
            std::cout << "\\multicolumn{" << (2 + Coloumn_Count) << "}{|c|}{" << gn << "} \\\\ \\hline\n";
        }

        void printAVG(const size_t &i, const size_t &j) {
            for (size_t k = 0, L = Coloumn_Count; k < L; ++k)
                std::cout << " & "<< rsdf[k].second[i].second[j].avg;
            std::cout << "\\\\ \\cline{2-" << (2 + Coloumn_Count) << "}\n";
            std::cout << " & min ";
        }

        void printMIN(const size_t &i, const size_t &j) {
            for (size_t k = 0, L = Coloumn_Count; k < L; ++k)
                std::cout << " & "<< rsdf[k].second[i].second[j].min;
            std::cout << "\\\\ \\cline{2-" << (2 + Coloumn_Count) << "}\n";
            std::cout << " & max ";
        }

        void printMAX(const size_t &i, const size_t &j) {
            for (size_t k = 0, L = Coloumn_Count; k < L; ++k)
                std::cout << " & "<< rsdf[k].second[i].second[j].max;
            std::cout << "\\\\ \\hline \n";
        }

        void printOneTestInGroup(const size_t &i, const size_t &j) {
            std::cout << rsdf[0].second[i].second[j].name << " & avg";
            printAVG(i, j);
            printMIN(i, j);
            printMAX(i, j);
        }
       
        /** @brief printTestResult - Вывод групп и вложенных тестов
         * */
        void printTestResult() {
            for (size_t i = 0, N = rsdf[0].second.size(); i <  N; ++i) {
                printGroupTitle(rsdf[0].second[i].first);
                for (size_t j = 0; M = rsdf[0].second[i].second.size(); j < M; ++j)
                    printOneTestInGroup(i, j);
            }
        }

        /** @brief printTableEnd - Вывод конца таблицы
         * */
        void printTableEnd() {
            std::cout << "\\end{tabular}" << std::endl;
        }

    public:
        ResultsOut() = default;

        /** @brief ResultsOut(const std::string &fn) - Считать результаты из файлов
         * */
        ResultsOut(const std::string &fn) {
            readResultFromCsv(fn);
        }
        
        /** @brief ResultsOut(const std::vector<std::string> &fns) - Считать группу результатов
         * */
        ResultsOut(const std::vector<std::string> &fns) {
            for (size_t i = 0, N = fns.size(); i < N; ++i)
                readResultFromCsv(fns[i]);
        }
        
        /** @brief ResultsOut(const std::string &CPU, const ResultDF &rdf) - записать при создании уже 1 результат
         * */
        ResultsOut(const std::pair<std::string, ResultDF> &prdf) {
            rsdf.push_back(prdf);
        }

        ~ResultsOut() {
            DataFrameFunction::clear_results_data_frame(rsdf);
        }

        /** @brief addResultDF - добавляет массив данных, к возможности вывода
         * */
        void addResultDF(const std::string &CPU, const ResultDF> &rdf) {
            rsdf.push_back(std::pair<std::string, ResultDf>(CPU, rdf));
        }

        /** outTexResults - Долго думал как красиво вывести данные, и на мой взгляд лучший вариант ввести их в tex таблицу,
         *                  функция ввыводит формат tex.
         * */
        void outTexResults() {
            // TODO: Проверка корректных данных, чтобы не была краша по памяти 
            printTableTitle();
            printTableHead();
            printTestResult();
            printTableEnd();
        }
        
        /** @brief readResultFromCsv - функция считывает один csv с данными для одного результата. 
         *                             В функцию необходимо передать полное имя ('Имя процессора/системы' + .csv).
         *  TODO: разбить функцию и сократить колличество строк в функции. 29.07.23 - лень делать
         * */
        int readResultFromCsv(const std::string &fn) {
            std::ifstream fin{fn, std::ios::binary | std::ios::in};
            if (!fin.is_open()) {
                return -1;
            }

            ResultDF rdf;
            std::pair<std::string, std::vector<ResultTest>> prdf
            std::string buf, gr, nm;
            ResultTest rt;
            while (fin.eof() == 0) {
                do {
                    fin >> buf;
                    if (buf != ";")
                        gp += (buf + " ");
                } while (buf != ";");
                gp.pop_back();

                if (prdf.first.empty()) {
                    prdf.first = gp;
                } else if (prdf.first == gp) {
                    gp.clear();
                } else {
                    rdf.push_back(prdf);
                    DataFrameFunction::clear_buffer_pair_rt(prdf);
                    prdf.first = gp;
                    gp.clear();
                }
                
                do {
                    fin >> buf;
                    if (buf != ";")
                        nm += (buf + " ");
                } while (buf != ";");
                nm.pop_back();
                rt.name = nm;

                fin >> rt.avg >> buf >> rt.min >> buf >> rt.max >> buf >> buf;
                prdf.second.push_back(rt);
                nm.clear();
                rt.clear();
                buf.clear();
            }

            rsdf.push_back(std::pair<std::string, ResultDF>>(std::string(fn.cbegin(), (fn.cend() - 4)), rdf));
        }

        /** @brief writeResultsToCsv - Функция записывает сохраненные данные в "csv"
         *  TODO: разбить функцию и сократить колличество строк в функции. 29.07.23 - лень делать
         * */
        int writeResultsToCsv() {
            if (rsdf.empty()) {
                return -1;
            }

            std::ofstream fout;
            for (size_t i = 0, N = rsdf.size(); i < N; ++i) {
                fout.open(std::string(rsdf[i].first + ".csv").c_str(), std::ios::binary | std::ios::out);
                if (!fout.is_open()) {
                    return -1;
                }
                
                auto rdf = rsdf[i].second;  // Не очень эффективно это, но лень прописывать rsdf[i].second каждый раз
                for (size_t j = 0, M = rdf.size(); j < M; ++j) {
                    for (size_t k = 0, L = rdf[j].second.size(); k < L; ++k) {
                        fout << rdf[j].first << " ; " 
                             << rdf[j].second[k].name << " ; " 
                             << rdf[j].second[k].avg << " ; " 
                             << rdf[j].second[k].min << " ; " 
                             << rdf[j].second[k].max << " ; " << endl;
                    }
                }
                fout.close();
            }
            return 0;
        }
    };

};  // StepanTestScriptLibrary


#endif // STEPANTESTSCRIPTLIBRARY_TIMETEST_H_
