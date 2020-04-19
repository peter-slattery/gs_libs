//
// File: win32_foldhaus_timing.h
// Author: Peter Slattery
// Creation Date: 2020-02-04
//
//
// NOTE: Relies on having imported foldhaus_platform.h prior to this file
//
#ifndef WIN32_FOLDHAUS_TIMING_H

typedef int           gswt_s32;
typedef long long int gswt_s64;


static gswt_s64
GetPerformanceFrequency (gswt_s32* ErrorOut = 0)
{
    LARGE_INTEGER Frequency;
    if (!QueryPerformanceFrequency(&Frequency))
    {
        gswt_s32 Error = GetLastError();
        if (ErrorOut != 0)
        {
            *ErrorOut = Error;
        }
    }
    return (gswt_s64)Frequency.QuadPart;
}

static gswt_s64
GetWallClock (gswt_s32* ErrorOut = 0)
{
#if 0
    gswt_s64 Result = __rdtsc();
    return Result;
#else
    LARGE_INTEGER Time;
    if (!QueryPerformanceCounter(&Time))
    {
        gswt_s32 Error = GetLastError();
        if (ErrorOut != 0)
        {
            *ErrorOut = Error;
        }
    }
    return (gswt_s64)Time.QuadPart;
#endif
}

static float
GetSecondsElapsed(gswt_s64 Start, gswt_s64 End, gswt_s64 PerformanceCountFrequency)
{
    float Result = ((float)(End - Start) / (float) PerformanceCountFrequency);
    return Result;
}

#define WIN32_FOLDHAUS_TIMING_H
#endif // WIN32_FOLDHAUS_TIMING_H