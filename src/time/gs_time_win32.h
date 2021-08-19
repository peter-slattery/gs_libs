/* date = May 22nd 2021 1:49 pm */

#ifndef GS_TIME_WIN32_H
#define GS_TIME_WIN32_H

PLATFORM_TICKS_GET_CURRENT(Win32TicksGetCurrent)
{
    LARGE_INTEGER Time;
    if (!QueryPerformanceCounter(&Time))
    {
        s32 Error = GetLastError();
        // TODO(Peter): I'm waiting to see an error actually occur here
        // to know what it could possibly be.
        InvalidCodePath;
    }
    return (u64)Time.QuadPart;
}

PLATFORM_SLEEP_FOR_MS(Win32SleepForMS)
{
    Sleep(MS);
}

internal gs_time_handler 
CreatePlatformTimeHandler()
{
    LARGE_INTEGER Frequency;
    if (!QueryPerformanceFrequency(&Frequency))
    {
        s32 Error = GetLastError();
        // TODO(Peter): I'm waiting to see an error actually occur here
        // to know what it could possibly be.
        InvalidCodePath;
    }
    s64 PerformanceFrequency = (s64)Frequency.QuadPart;
    
    return TimeHandlerCreate(PerformanceFrequency,
                             Win32TicksGetCurrent,
                             Win32SleepForMS,
                             0);
}

#endif //GS_TIME_WIN32_H
