#ifndef STEPANTESTSCRIPTLIBRARY_GETCPUTIME_H_
#define STEPANTESTSCRIPTLIBRARY_GETCPUTIME_H_

#if defined(_WIN32)
#include <Windows.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>

#else
#error "Unable to define getCPUTime( ) for an unknown OS."
#endif

namespace STSL {
    /** @breif getCPUTime - \
     * Возвращает количество процессорного времени, используемого текущим процессом, \
     * в секундах или -1,0, если произошла ошибка. \
     * Returns the amount of CPU time used by the current process, \
     * in seconds, or -1.0 if an error occurred.
     */
    double getCPUTime();
};  // StepanTestScriptLibrary

#endif  // STEPANTESTSCRIPTLIBRARY_GETCPUTIME_H_
