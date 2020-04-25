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

#ifndef GS_TYPES_H
# error "gs_time.h relies on gs_types.h. Please include that file first."
#endif

typedef struct gs_time_info gs_time_info;
typedef s64 gs_time_stamp;

#define NANOS_PER_USEC 1000ULL
#define NANOS_PER_MILLISEC 1000ULL * NANOS_PER_USEC
#define NANOS_PER_SEC 1000ULL * NANOS_PER_MILLISEC

internal gs_time_info gs_InitializeTimeInfo(s32* ErrorOut);
internal gs_time_stamp gs_GetWallClock(s32* ErrorOut);
internal r64 gs_GetSecondsElapsed(gs_time_info Info, gs_time_stamp Start, gs_time_stamp End);

#ifdef GS_CSTDLIB
#endif

#if defined(GS_TIME_WIN32) || defined(GS_PLATFORM_WIN32)

struct gs_time_info
{
    gs_time_stamp PerformanceCountFrequency;
};

internal void
gs_TimeGetLastError(s32* ErrorOut = 0)
{
    s32 Error = GetLastError();
    if (ErrorOut != 0)
    {
        *ErrorOut = Error;
    }
}

internal gs_time_info
gs_InitializeTimeInfo(s32* ErrorOut = 0)
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

internal gs_time_stamp
gs_GetWallClock(s32* ErrorOut = 0)
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

internal r64
gs_GetSecondsElapsed(gs_time_info TimeInfo, gs_time_stamp Start, gs_time_stamp End)
{
    r64 Result = ((r64)(End - Start) / (r64)TimeInfo.PerformanceCountFrequency);
    return Result;
}

internal void
gs_Sleep(u64 SleepTime)
{
    Sleep(SleepTime);
}

#endif

#if defined(GS_TIME_OSX) || defined(GS_PLATFORM_OSX)

struct gs_time_info
{
    gs_time_stamp StartTimeAbsolute;
    mach_timebase_info_data_t MachTimeInfo;
};

internal gs_time_info
gs_InitializeTimeInfo(s32* ErrorOut = 0)
{
    gs_time_info Result = {0};
    Result.StartTimeAbsolute = mach_absolute_time();
	mach_timebase_info(&Result.MachTimeInfo);
    return Result;
}

internal gs_time_stamp
gs_GetWallClock(s32* ErrorOut = 0)
{
    gs_time_stamp Result = mach_absolute_time();
    return Result;
}

internal r64
gs_GetSecondsElapsed(gs_time_info Info, gs_time_stamp Start, gs_time_stamp End)
{
    r64 Result = 0;
	r64 Elapsed = (r64)(End - Start);
	Result = Elapsed * (r64)(TimeInfo.MachTimeInfo.numer) / (r64)NANOS_PER_SEC / (r64)TimeInfo.MachTimeInfo.denom;
	return Result;
}

internal void
gs_Sleep(u64 SleepTime)
{
    InvalidCodePath;
}

#endif

internal gs_time_stamp
gs_SleepUntilTargetReached(gs_time_info TimeInfo, gs_time_stamp Start, r64 TargetSecondsPerFrame)
{
    s64 FrameEndTime = gs_GetWallClock();
    r64 SecondsElapsed = gs_GetSecondsElapsed(TimeInfo, Start, FrameEndTime);
    while (SecondsElapsed < TargetSecondsPerFrame)
    {
        u32 SleepTime = 1000.0 * (TargetSecondsPerFrame - SecondsElapsed);
        gs_Sleep(SleepTime);
        SecondsElapsed = gs_GetSecondsElapsed(TimeInfo, Start, gs_GetWallClock());
    }
    s64 EndTime = gs_GetWallClock();
    return EndTime;
}

#define GS_TIME_H
#endif // GS_TIME_H