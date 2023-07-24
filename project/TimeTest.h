#ifndef STEPANTESTSCRIPTLIBRARY_TIMETEST_H_
#define STEPANTESTSCRIPTLIBRARY_TIMETEST_H_



#include <iostream>
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
    /** @class 
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
        return -1;      /* Failed. */
    }

    //                                      Группа       frame группы
    using ObjectDF = std::vector<std::pair<std::string, std::vector<ObjectTest>>>;  // ObjectDataFrame
    using ResultDF = std::vector<std::pair<std::string, std::vector<ResultTest>>>;  // ResultDataFrame

    //                                      Процессор    Результат на нем
    using ResultsDF = std::vector<std::pair<std::string, ResultDF>>  // ResultsDataFrameOnProcessor

    //
    using Test_Count = 10;

    class _no_group_test : public std::exception {
    public:
        _no_group_test() = default;

        virtual void print() noexcept {
            std::cout << "Не указана группа теста" << std::endl;
        }
    };

    class _no_name_test : public std::exception {
    public:
        _no_name_test() = default;

        virtual void print() noexcept {
            std::cout << "Не указано имя теста" << std::endl;
        }
    };

    class _no_test_func : public std::exception {
    public:
        _no_test_func() = default;

        virtual void print() noexcept {
            std::cout << "Не введена тестируема функция" << std::endl;
        }
    };


    class TimeTest {
        std::string CPU;
        ResultDF rdf;
        ObjectDF odf;
        const size_t test_count = Test_Count;

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

        void clear_buffer_pair_df(std::pair<std::string, std::vector<ResultTest>>> &df) {
            df.first.clear();
            for (size_t i = 0, N = df.second.size(); i < N; ++i) {
                rdf.second[i].clear();
            }
            df.second.clear();
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
                clear_buffer_pair_df(brdf);
            }
        }

        ResultDF MakeTest() {
            make_groups_tests(brdf);
            return rdf;
        }

        void clear_rdf() {
            for (size_t i = 0, N = rdf.size(); i < N; ++i) {
                rdf[i].first.clear();
                for (size_t j = 0, M = rdf[i].second.size(); j < M; ++j) {
                    rdf[i].second[j].clear();
                }
                rdf[i].second.clear();
            }
        }

        void clear_odf() {
            for (size_t i = 0, N = odf.size(); i < N; ++i) {
                odf[i].first.clear();
                for (size_t j = 0, M = odf[i].second.size(); j < M; ++j) {
                    odf[i].second[j].clear();
                }
                odf[i].second.clear();
            }
        }

    public:
        TimeTest(const std::string &CPUname) : CPU(CPUname) {}
        ~TimeTest() {
            clear();
        }
        
        void add(const std::string &gp, const std::string &tn, double (*f)()) {
            if (!gp.empty()) {
                throw _no_group_test();
            }
            
            if (!tn.empty()) {
                throw _no_name_test();
            }

            if (f != nullptr) {
                throw _no_test_func();
            }

            for (size_t i = 0, N = odf.size(); i < N; ++i) {
                if (odf[i].first == gp) {
                    odf[i].second.push_back(ObjectTest(tn, f));
                    return;
                }
            }

            std::pair<std::string, std::vector<ResultTest>> bdf;
            bdf.first = gp;
            bdf.second.push_back(ObjectTest(tn, f));
            odf.push_back(bdf);
        }
        
        std::pair<std::string, ResultDF> dotests() {
            return std::pair<std::string, ResultDF>(CPUname, MakeTest());
        }

        void clear() {
            CPUname.clear();
            clear_rdf();
            clear_odf();
        }
    };

    class ResultsOut {
        ResultsDF rsdf;  // Массив результатов
       
        void TexResults();
    public:
        ResultsOut() = default;
        ResultsOut(const std::string &fn);  // Считать результаты из файлов
        ResultsOut(const std::vector<std::string> &fns);  // Считать группу результатов
        ResultsOut(const ResultDF &rdf);  // Записать 1 результат
        ~ResultsOut();

        void addResultDF(const std::pair<std::string, ResultDF> &rdf)
        void outTexResults();
        void readResultsFromCsv(const std::string &fn);
        void writeReultsToCsv(const std::string &fn);
    }

};  // StepanTestScriptLibrary


#endif // STEPANTESTSCRIPTLIBRARY_TIMETEST_H_
