//
// File: gs_time.h
// Author: Peter Slattery
// Creation Date: 2020-03-30
//
//
//
// Usage:
// To use, include this file in ONE file in your program, and define one of values
// listed below, based on which implementation you'd like to use
//
// Defines
// GS_TIME_CSTDLIB - includes the C standard libary implementation.
//
// GS_TIME_WIN32
// Depends on: <windows.h>
//
// GS_TIME_OSX
// Depends on: <mach/mach_time.h>
//
#ifndef GS_TIME_H

typedef struct gs_time_info gs_time_info;
typedef long long int gs_time_stamp;

#define NANOS_PER_USEC 1000ULL
#define NANOS_PER_MILLISEC 1000ULL * NANOS_PER_USEC
#define NANOS_PER_SEC 1000ULL * NANOS_PER_MILLISEC

static gs_time_info gs_InitializeTimeInfo(int* ErrorOut);
static gs_time_stamp gs_GetWallClock(int* ErrorOut);
static double gs_GetSecondsElapsed(gs_time_info Info, gs_time_stamp Start, gs_time_stamp End);

#ifdef GS_CSTDLIB
#endif

#if defined(GS_TIME_WIN32) || defined(GS_PLATFORM_WIN32)

struct gs_time_info
{
    gs_time_stamp PerformanceCountFrequency;
};

static void
gs_TimeGetLastError(int* ErrorOut = 0)
{
    int Error = GetLastError();
    if (ErrorOut != 0)
    {
        *ErrorOut = Error;
    }
}

static gs_time_info
gs_InitializeTimeInfo(int* ErrorOut = 0)
{
    gs_time_info Result = {0};
    LARGE_INTEGER Frequency;
    if (QueryPerformanceFrequency(&Frequency))
    {
        Result.PerformanceCountFrequency = (gs_time_stamp)Frequency.QuadPart;
    }
    else
    {
        gs_TimeGetLastError(ErrorOut);
    }
    return Result;
}

static gs_time_stamp
gs_GetWallClock(int* ErrorOut = 0)
{
    gs_time_stamp Result = 0;
    LARGE_INTEGER Time;
    if (QueryPerformanceCounter(&Time))
    {
        Result = (gs_time_stamp)Time.QuadPart;
    }
    else
    {
        gs_TimeGetLastError(ErrorOut);
    }
    return Result;
}

static double
gs_GetSecondsElapsed(gs_time_info TimeInfo, gs_time_stamp Start, gs_time_stamp End)
{
    double Result = ((double)(End - Start) / (double)TimeInfo.PerformanceCountFrequency);
    return Result;
}

#endif

#if defined(GS_TIME_OSX) || defined(GS_PLATFORM_OSX)

struct gs_time_info
{
    gs_time_stamp StartTimeAbsolute;
    mach_timebase_info_data_t MachTimeInfo;
};

static gs_time_info
gs_InitializeTimeInfo(int* ErrorOut = 0)
{
    gs_time_info Result = {0};
    Result.StartTimeAbsolute = mach_absolute_time();
	mach_timebase_info(&Result.MachTimeInfo);
    return Result;
}

static gs_time_stamp
gs_GetWallClock(int* ErrorOut = 0)
{
    gs_time_stamp Result = mach_absolute_time();
    return Result;
}

static double
gs_GetSecondsElapsed(gs_time_info Info, gs_time_stamp Start, gs_time_stamp End)
{
    double Result = 0;
	double Elapsed = (double)(End - Start);
	Result = Elapsed * (double)(TimeInfo.MachTimeInfo.numer) / (double)NANOS_PER_SEC / (double)TimeInfo.MachTimeInfo.denom;
	return Result;
}

#endif

#define GS_TIME_H
#endif // GS_TIME_H