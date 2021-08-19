/* date = May 22nd 2021 1:37 pm */

#ifndef GS_TIME_H
#define GS_TIME_H

#define PLATFORM_TICKS_GET_CURRENT(name) u64 name(u8* UserData)
#define PLATFORM_SLEEP_FOR_MS(name) void name(u64 MS, u8* UserData)

typedef PLATFORM_TICKS_GET_CURRENT(platform_ticks_get_current);
typedef PLATFORM_SLEEP_FOR_MS(platform_sleep_for_ms);

typedef struct gs_time_handler
{
    r64 TicksPerSecond;
    
    platform_ticks_get_current* PTicksGetCurrent;
    platform_sleep_for_ms* PSleepForMS;
    
    u8* UserData;
} gs_time_handler;

PLATFORM_TICKS_GET_CURRENT(PTicksGetCurrentNoOp)
{
    return 0;
}

PLATFORM_SLEEP_FOR_MS(PSleepForMSNoOp)
{
    return;
}

// NOTE(PS): This just makes sure name exists, and if not, assigns
// the NoOp version of name instead.
// ie. if name = PFileSeek, then this evaluates to 
//     (PFileSeek) ? (PFileSeek) : (PFileSeekNoOp)
#define TIME_HANDLER_SAFE_PROC(name) (name) ? (name) : (name ## NoOp)

internal gs_time_handler
TimeHandlerCreate(r64 TicksPerSecond, 
                  platform_ticks_get_current* PTicksGetCurrent,
                  platform_sleep_for_ms* PSleepForMS,
                  u8* UserData)
{
    Assert(TicksPerSecond != 0);
    
    gs_time_handler Result = {
        .TicksPerSecond = TicksPerSecond,
        .PTicksGetCurrent   = TIME_HANDLER_SAFE_PROC(PTicksGetCurrent),
        .PSleepForMS   = TIME_HANDLER_SAFE_PROC(PSleepForMS),
        .UserData = UserData
    };
    
    return Result;
}

internal u64
TicksGetCurrent(gs_time_handler TM)
{
    return TM.PTicksGetCurrent(TM.UserData);
}

internal r64
TicksToSeconds(gs_time_handler TM, u64 Ticks)
{
    r64 Result = (r64)Ticks / TM.TicksPerSecond;
    return Result;
}

internal r64
TickRangeToSeconds(gs_time_handler TM, u64 Start, u64 End)
{
    Assert(Start <= End);
    u64 Ticks = End - Start;
    return TicksToSeconds(TM, Ticks);
}

internal void
SleepForMS(gs_time_handler TM, u64 MS)
{
    TM.PSleepForMS(MS, TM.UserData);
}

#ifdef GS_PLATFORM_IMPLEMENTATION

internal gs_time_handler CreatePlatformTimeHandler();

# if PLATFORM_WINDOWS
#  include "./gs_time_win32.h"
# elif PLATFORM_OSX
#  include "./gs_time_osx.h"
# elif PLATFORM_LINUX
#  include "./gs_time_linux.h"
# endif

#endif

#endif //GS_TIME_H
