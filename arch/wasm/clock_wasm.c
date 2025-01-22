/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 *
 *    Copyright 2016-2017 (c) Fraunhofer IOSB (Author: Julius Pfrommer)
 *    Copyright 2017 (c) Stefan Profanter, fortiss GmbH
 *    Copyright 2017 (c) Thomas Stalder, Blue Time Concept SA
 */

/* Note that the EventLoop plugin provides its own internal time source (which
 * is typically just the normal system time). All internal access to the time
 * source should be through the EventLoop. The below is therefore for developer
 * convenience to just use UA_DateTime_now(). */

// #if defined(UA_ARCHITECTURE_WASM)

#include <open62541/types.h>

#include <stdio.h>
#include <time.h>
#include <sys/time.h>


UA_DateTime UA_DateTime_now(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * UA_DATETIME_SEC) +
        (tv.tv_usec * UA_DATETIME_USEC) +
        UA_DATETIME_UNIX_EPOCH;
}

/* Credit to https://stackoverflow.com/questions/13804095/get-the-time-zone-gmt-offset-in-c */
UA_Int64 UA_DateTime_localTimeUtcOffset(void) {
    time_t rawtime = time(NULL);
    //struct tm gbuf;
    //struct tm *ptm = gmtime_r(&rawtime, &gbuf);
    struct tm *ptm = gmtime(&rawtime);
    /* Request mktime() to look up dst in timezone database */
    ptm->tm_isdst = -1;
    time_t gmt = mktime(ptm);
    return (UA_Int64) (difftime(rawtime, gmt) * UA_DATETIME_SEC);
}

UA_DateTime UA_DateTime_nowMonotonic(void) {
    return UA_DateTime_localTimeUtcOffset();
}

/*
UA_DateTime UA_DateTime_nowMonotonic(void) {
#if defined(__APPLE__) || defined(__MACH__)
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    return (mts.tv_sec * UA_DATETIME_SEC) + (mts.tv_nsec / 100);
#elif !defined(CLOCK_MONOTONIC_RAW)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * UA_DATETIME_SEC) + (ts.tv_nsec / 100);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (ts.tv_sec * UA_DATETIME_SEC) + (ts.tv_nsec / 100);
#endif
}
*/

// #endif 
