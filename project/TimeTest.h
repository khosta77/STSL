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
    }; /** @class ObjectTest - Сохраняем функцию в которой проходит тест в отдельную структуру, \
     *                      из которой позже произведем ее вызов
     * */
    struct ObjectTest {
        std::string name;   // Имя функции(тесты)
        double (*foo)();    // Сама функция, у нее есть своя структура. Об этом в другом месте

        ObjectTest(const std::string& n, double (*f)()) : name(n), foo(f) {} 

        ~ObjectTest() {
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
    using ResultDF = std::vector<std::pair<std::string, std::vector<ResultTest>>>;  // ResultDataFrame

    //                                      Процессор    Результат на нем
    using ResultsDF = std::vector<std::pair<std::string, ResultDF>>  // ResultsDataFrameOnProcessor
    using ObjectDF = std::vector<std::pair<std::string, std::vector<ObjectTest>>>;  // ObjectDataFrame
 /*
    class OtherResults {
        ResultsDF rsdf;

        // TODO
    public:
        OtherResults() = default;  // TODO
        ~OtherResults() = default;  // TODO

        void writeCSVs();  // TODO
        void readCSV(const std::string &fn);  // TODO
    }
*/
    class ResultLaTexOut {
        // TODO
    public:
        ResultLaTexOut() = default;

        void outTexResults(const ResultsDF &rsdf);  // TODO
    }

    class TimeTest {
        ResultDF rdf;
        ObjectDF odf;

        ResultDF MakeTest(const ObjectDF &df);


    public:
        TimeTest();
        ~TimeTest() = default;

        void outTexResult();
        
        void writeCSV(const std::string &fn);
        void 
    };
};  // StepanTestScriptLibrary


#endif // STEPANTESTSCRIPTLIBRARY_TIMETEST_H_
