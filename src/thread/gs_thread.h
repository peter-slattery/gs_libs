/* date = May 15th 2021 2:22 pm */

#ifndef GS_THREAD_H
#define GS_THREAD_H

#ifndef GS_TYPES_H
# error "gs_thread relies on gs_types"
#endif

#ifndef Assert
# define Assert
#endif

#if !defined(GS_THREAD_PROFILE_FUNC)
# define GS_THREAD_PROFILE_FUNC
#endif

typedef enum
{
  ThreadCreate_Suspended   = 0x1,
  ThreadCreate_ReserveSize = 0x2,
} gs_thread_creation_flags;

typedef enum
{
  ThreadStatus_Invalid,
  ThreadStatus_Active,
} gs_thread_status;

// NOTE(PS): This should be a number of bytes larger than any of our platforms
// require to store a thread handle
#define GS_THREAD_PLATFORM_DATA_SIZE 8

typedef struct gs_thread_handle
{
  u64 SystemId;
  u64 Id_;
  u8 PlatformData[GS_THREAD_PLATFORM_DATA_SIZE];
} gs_thread_handle;

typedef struct gs_thread_proc_ctx gs_thread_proc_ctx;
typedef s32 gs_thread_proc(gs_thread_proc_ctx* Ctx);
typedef s32 gs_thread_entry_proc(u8* Arg);

typedef struct gs_thread_manager gs_thread_manager;
struct gs_thread_proc_ctx
{
  u64 Index;
  b8 Running;
  gs_thread_manager* ThreadManager;
  u8* ThreadProcParam;
  gs_thread_proc* ThreadProc;
};

#define PLATFORM_THREAD_GET_ID(name) u64 name(u8* UserData)
#define PLATFORM_THREAD_GET_HANDLE(name) gs_thread_handle name(u8* UserData)
#define PLATFORM_THREAD_GET_STATUS(name) u32 name(gs_thread_handle* ThreadHandle, u8* UserData)
#define PLATFORM_THREAD_RESUME(name) void name(gs_thread_handle* ThreadHandle, u8* UserData)
#define PLATFORM_THREAD_CREATE(name) gs_thread_handle name(u64 StackSize, gs_thread_entry_proc* ThreadProc, u8* ThreadProcParam, u8 Flags, u8* UserData)
#define PLATFORM_THREAD_KILL(name) bool name(gs_thread_handle* ThreadHandle, u32 ExitCode, u8* UserData)

// Intrinsics

#define PLATFORM_INTERLOCKED_CMP_EXCHNG_32(name) s32 name(s32* Destination, s32 NewValue, s32 OldValue, bool* SuccessOut, u8* UserData)
#define PLATFORM_INTERLOCKED_CMP_EXCHNG_64(name) s32 name(s64* Destination, s64 NewValue, s64 OldValue, bool* SuccessOut, u8* UserData)

typedef PLATFORM_THREAD_GET_ID(platform_thread_get_id);
typedef PLATFORM_THREAD_GET_HANDLE(platform_thread_get_handle);
typedef PLATFORM_THREAD_GET_STATUS(platform_thread_get_status);
typedef PLATFORM_THREAD_RESUME(platform_thread_resume);
typedef PLATFORM_THREAD_CREATE(platform_thread_create);
typedef PLATFORM_THREAD_KILL(platform_thread_kill);

typedef PLATFORM_INTERLOCKED_CMP_EXCHNG_32(platform_interlocked_cmp_exchng_32);
typedef PLATFORM_INTERLOCKED_CMP_EXCHNG_64(platform_interlocked_cmp_exchng_64);

struct gs_thread_manager
{
  u64 ThreadCountMax;
  gs_thread_handle* ThreadHandleTable;
  
  gs_thread_proc_ctx* ThreadCtxArray;
  u64 ThreadCtxArrayUsed;
  
  platform_thread_get_id* PThreadGetId;
  platform_thread_get_handle* PThreadGetHandle;
  platform_thread_get_status* PThreadGetStatus;
  platform_thread_resume* PThreadResume;
  platform_thread_create* PThreadCreate;
  platform_thread_kill* PThreadKill;
  
  platform_interlocked_cmp_exchng_32* PInterlockedCmpExchng32;
  platform_interlocked_cmp_exchng_64* PInterlockedCmpExchng64;
  
  u8* UserData;
};

PLATFORM_THREAD_GET_ID(PThreadGetIdNoOp)
{
  return 0;
}

PLATFORM_THREAD_GET_HANDLE(PThreadGetHandleNoOp)
{
  return (gs_thread_handle){0};
}

PLATFORM_THREAD_GET_STATUS(PThreadGetStatusNoOp)
{
  return ThreadStatus_Invalid;
}

PLATFORM_THREAD_RESUME(PThreadResumeNoOp)
{
  return;
}

PLATFORM_THREAD_CREATE(PThreadCreateNoOp)
{
  return (gs_thread_handle){0};
}

PLATFORM_THREAD_KILL(PThreadKillNoOp)
{
  return false;
}

PLATFORM_INTERLOCKED_CMP_EXCHNG_32(PInterlockedCmpExchng32NoOp)
{
  return 0;
}

PLATFORM_INTERLOCKED_CMP_EXCHNG_64(PInterlockedCmpExchng64NoOp)
{
  return 0;
}

#define THREAD_HANDLER_SAFE_PROC(name) (name) ? (name) : (name ## NoOp)

internal gs_thread_manager
ThreadManagerCreate(u64 ThreadCountMax,
                    
                    // NOTE(PS): these should be a pointers to arrays
                    // that are of length ThreadCountMax
                    gs_thread_handle* ThreadHandleTable,
                    gs_thread_proc_ctx* ThreadCtxArray,
                    
                    platform_thread_get_id* PThreadGetId,
                    platform_thread_get_handle* PThreadGetHandle,
                    platform_thread_get_status* PThreadGetStatus,
                    platform_thread_resume* PThreadResume,
                    platform_thread_create* PThreadCreate,
                    platform_thread_kill* PThreadKill,
                    platform_interlocked_cmp_exchng_32* PInterlockedCmpExchng32,
                    platform_interlocked_cmp_exchng_64* PInterlockedCmpExchng64,
                    
                    u8* UserData)
{
  // NOTE(PS): There is always one thread - the main thread
  Assert(ThreadCountMax >= 1);
  
  gs_thread_manager Result = {
    .ThreadCountMax = ThreadCountMax,
    .ThreadHandleTable = ThreadHandleTable,
    .ThreadCtxArray = ThreadCtxArray,
    
    .PThreadGetId = THREAD_HANDLER_SAFE_PROC(PThreadGetId),
    .PThreadGetHandle = THREAD_HANDLER_SAFE_PROC(PThreadGetHandle),
    .PThreadGetStatus = THREAD_HANDLER_SAFE_PROC(PThreadGetStatus),
    .PThreadResume = THREAD_HANDLER_SAFE_PROC(PThreadResume),
    .PThreadCreate = THREAD_HANDLER_SAFE_PROC(PThreadCreate),
    .PThreadKill = THREAD_HANDLER_SAFE_PROC(PThreadKill),
    .PInterlockedCmpExchng32 = THREAD_HANDLER_SAFE_PROC(PInterlockedCmpExchng32),
    .PInterlockedCmpExchng64 = THREAD_HANDLER_SAFE_PROC(PInterlockedCmpExchng64),
  };
  return Result;
};

internal bool
ThreadHandleIsValid(gs_thread_handle ThreadHandle)
{
  GS_THREAD_PROFILE_FUNC;
  bool Result = ThreadHandle.SystemId != 0;
  return Result;
}

internal u64
ThreadSystemIdToNiceId(gs_thread_manager* T, u64 SystemId)
{
  GS_THREAD_PROFILE_FUNC;
  u64 Result = 0;
  // TODO(PS): This is slow - once we have another platform on here
  // lets make this a hash table? although, values of T.ThreadCountMax
  // will never be above like. 256 even on super computers so does it 
  // really matter if this is excessive?
  for (u64 i = 0; i < T->ThreadCountMax; i++)
  {
    if (T->ThreadHandleTable[i].SystemId == SystemId)
    {
      Result = i;
      break;
    }
  }
  return Result;
}

internal u64
ThreadGetId(gs_thread_manager* T)
{
  // NOTE(PS): we can't profile this function because BeginScope calls ThreadGetId,
  // making an endless loop
  //GS_THREAD_PROFILE_FUNC;
  u64 SystemId = T->PThreadGetId(T->UserData);
  u64 Result = ThreadSystemIdToNiceId(T, SystemId);
  return Result;
}

internal gs_thread_handle
ThreadGetHandle(gs_thread_manager* T)
{
  GS_THREAD_PROFILE_FUNC;
  gs_thread_handle Result = T->PThreadGetHandle(T->UserData);
  Result.Id_ = ThreadSystemIdToNiceId(T, Result.SystemId);
  return Result;
}

internal u32
ThreadGetStatus(gs_thread_manager* T, gs_thread_handle* ThreadHandle)
{
  GS_THREAD_PROFILE_FUNC;
  //return T->PThreadGetStatus(ThreadHandle, T->UserData);
  u32 Result = ThreadStatus_Invalid;
  if (T->ThreadHandleTable[ThreadHandle->Id_].SystemId != 0)
  {
    gs_thread_proc_ctx Ctx = T->ThreadCtxArray[ThreadHandle->Id_];
    Result = Ctx.Running ? ThreadStatus_Active : ThreadStatus_Invalid;
  }
  return Result;
}

internal void
ThreadResume(gs_thread_manager* T, gs_thread_handle* ThreadHandle)
{
  GS_THREAD_PROFILE_FUNC;
  return T->PThreadResume(ThreadHandle, T->UserData);
}

internal gs_thread_proc_ctx*
ThreadManagerCreateContext(gs_thread_manager* T, gs_thread_proc* Proc, u8* ThreadProcParam)
{
  GS_THREAD_PROFILE_FUNC;
  u64 Index = 0;
  if (T->ThreadCtxArrayUsed + 1 < T->ThreadCountMax)
  {
    Index = ++T->ThreadCtxArrayUsed; // preincrement because Index 0 is the main thread
  }
  else
  {
    for (u64 i = 1; i < T->ThreadCountMax; i++)
    {
      if (T->ThreadHandleTable[i].SystemId == 0)
      {
        Index = i;
        break;
      }
    }
  }
  
  // NOTE(PS): Index 0 is reserved for the main thread - we should never be creating
  // a thread context for the main thread, hence htis assert
  Assert(Index != 0);
  
  gs_thread_proc_ctx* Result = T->ThreadCtxArray + Index;
  Result->Index = Index;
  Result->ThreadProc = Proc;
  Result->ThreadProcParam = ThreadProcParam;
  Result->ThreadManager = T;
  return Result;
}

internal s32
ThreadEntryProc_(u8* ArgData)
{
  GS_THREAD_PROFILE_FUNC;
  gs_thread_proc_ctx* Ctx = (gs_thread_proc_ctx*)ArgData;
  s32 Result = Ctx->ThreadProc(Ctx);
  
  u8* UserData = Ctx->ThreadManager->UserData;
  bool Success = false;
  // Ctx->Running = false
  Ctx->ThreadManager->PInterlockedCmpExchng32((s32*)&Ctx->Running, (b32)false, (b32)true, &Success, UserData);
  Assert(Success);
  
  // Clear the handle stored in ThreadHandleTable
  gs_thread_handle* Handle = Ctx->ThreadManager->ThreadHandleTable + Ctx->Index;
  u64 CurrentId = Handle->SystemId;
  Ctx->ThreadManager->PInterlockedCmpExchng64((s64*)&Handle->SystemId, (u64)0, (u64)CurrentId, &Success, UserData);
  Assert(Success);
  
  return Result;
}

internal gs_thread_handle
ThreadCreate(gs_thread_manager* T, u64 StackSize, gs_thread_proc* ThreadProc, u8* ThreadProcParam, u8 Flags)
{
  GS_THREAD_PROFILE_FUNC;
  // TODO(PS): might be a good idea to stipulate that new threads are only created on the main thread
  // it seems like a lot of work to make the Thread Manager thread safe.
  gs_thread_proc_ctx* Ctx = ThreadManagerCreateContext(T, ThreadProc, ThreadProcParam);
  
  gs_thread_handle Result = T->PThreadCreate(StackSize, ThreadEntryProc_, (u8*)Ctx, Flags, T->UserData);
  Result.Id_ = Ctx->Index;
  
  T->ThreadHandleTable[Ctx->Index] = Result;
  
  Ctx->Running = true;
  ThreadResume(T, &Result);
  
  return Result;
}

internal bool
ThreadKill(gs_thread_manager* T, gs_thread_handle* ThreadHandle, u32 ExitCode)
{
  GS_THREAD_PROFILE_FUNC;
  // remove the system handle form the SystemThreadIdTable so we can use that
  // nice index again
  T->ThreadHandleTable[ThreadHandle->Id_] = (gs_thread_handle){0};
  return T->PThreadKill(ThreadHandle, ExitCode, T->UserData);
}

internal s32
InterlockedCmpExchng32(gs_thread_manager* T, s32* Destination, s32 NewValue, s32 OldValue, bool* SuccessOut)
{
  GS_THREAD_PROFILE_FUNC;
  return T->PInterlockedCmpExchng32(Destination, NewValue, OldValue, SuccessOut, T->UserData);
}

internal s64
InterlockedCmpExchng64(gs_thread_manager* T, s64* Destination, s64 NewValue, s64 OldValue, bool* SuccessOut)
{
  GS_THREAD_PROFILE_FUNC;
  return T->PInterlockedCmpExchng64(Destination, NewValue, OldValue, SuccessOut, T->UserData);
}

#ifdef GS_PLATFORM_IMPLEMENTATION

internal gs_thread_manager CreatePlatformThreadManagerBuffered(u64 ThreadCountMax, gs_thread_handle* ThreadHandleTable, gs_thread_proc_ctx* ThreadCtxArray);
internal gs_thread_manager CreatePlatformThreadManager(u64 ThreadCountMax);
internal void CleanupPlatformThreadManager(gs_thread_manager* T);

# if PLATFORM_WINDOWS
#  include "./gs_thread_win32.h"
# elif PLATFORM_OSX
#  include "./gs_thread_osx.h"
# elif PLATFORM_LINUX
#  include "./gs_thread_linux.h"
# endif

#endif

#endif //GS_THREAD_H
