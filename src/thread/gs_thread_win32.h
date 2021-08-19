/* date = May 20th 2021 11:55 am */

#ifndef GS_THREAD_WIN32_H
#define GS_THREAD_WIN32_H

internal HANDLE*
Win32GetSystemHandle(gs_thread_handle* ThreadHandle)
{
  HANDLE* Win32Handle = (HANDLE*)ThreadHandle->PlatformData;
  return Win32Handle;
}

internal DWORD 
Win32ConvertThreadCreationFlags(u8 GSThreadCreationFlags)
{
  DWORD Result = 0;
  if (HasFlag(GSThreadCreationFlags, ThreadCreate_Suspended))
  {
    Result |= CREATE_SUSPENDED;
  }
  if (HasFlag(GSThreadCreationFlags, ThreadCreate_ReserveSize))
  {
    Result |= STACK_SIZE_PARAM_IS_A_RESERVATION;
  }
  return Result;
}

PLATFORM_THREAD_GET_ID(Win32GetThreadId)
{
  u64 Result = 0;
  DWORD ThreadId = GetCurrentThreadId();
  Result = (u64)ThreadId;
  return Result;
}

PLATFORM_THREAD_GET_HANDLE(Win32GetThreadHandle)
{
  gs_thread_handle Result = {0};
  Result.SystemId = Win32GetThreadId(UserData);
  HANDLE* Win32Handle = (HANDLE*)Result.PlatformData;
  *Win32Handle = GetCurrentThread();
  return Result;
}

PLATFORM_THREAD_GET_STATUS(Win32GetThreadStatus)
{
  u32 Result = 0;
  
  HANDLE* Win32Handle = Win32GetSystemHandle(ThreadHandle);
  if (*Win32Handle != INVALID_HANDLE_VALUE)
  {
    u32 ExitCode = 0;
    DWORD WaitMillis = 0;
    DWORD Signal = WaitForSingleObject(*Win32Handle, WaitMillis);
    if (Signal == WAIT_FAILED)
    {
      u32 Error = GetLastError();
      InvalidCodePath;
    }
    else if (Signal != WAIT_OBJECT_0)
    {
      Result = ThreadStatus_Active;
    }
  }
  
  return Result;
}

PLATFORM_THREAD_RESUME(Win32ThreadResume)
{
  HANDLE* Win32Handle = (HANDLE*)ThreadHandle->PlatformData;
  DWORD PrevSuspendCount = ResumeThread(*Win32Handle);
  if (PrevSuspendCount == -1)
  {
    s32 Error = GetLastError();
    switch (Error)
    {
      InvalidDefaultCase;
    }
  }
}

PLATFORM_THREAD_CREATE(Win32ThreadCreate)
{
  gs_thread_handle Result = {0};
  
  LPSECURITY_ATTRIBUTES SecurityAttrs = 0;
  LPTHREAD_START_ROUTINE StartRoutine = (LPTHREAD_START_ROUTINE)ThreadProc;
  void* StartRoutineParameter = (void*)ThreadProcParam;
  DWORD CreationFlags = CREATE_SUSPENDED;
  DWORD ThreadIdOut = 0;
  
  SetLastError(NO_ERROR);
  HANDLE ThreadHandle = CreateThread(SecurityAttrs,
                                     (SIZE_T)StackSize,
                                     StartRoutine,
                                     StartRoutineParameter,
                                     CreationFlags,
                                     &ThreadIdOut);
  
  s32 Error = GetLastError();
  if (ThreadHandle != NULL)
  {
    Result.SystemId = (u64)ThreadIdOut;
    
    HANDLE* ResultHandle = (HANDLE*)Result.PlatformData;
    *ResultHandle = ThreadHandle;
  }
  else
  {
    // TODO(PS): There is no information on what errors this can
    // throw in the docs.
    InvalidCodePath;
  }
  
  return Result;
}

PLATFORM_THREAD_KILL(Win32ThreadKill)
{
  bool Result = false;
  
  HANDLE* Win32Handle = (HANDLE*)ThreadHandle->PlatformData;
  BOOL Success = TerminateThread(*Win32Handle, ExitCode);
  if (Success)
  {
    //*ThreadHandle = (gs_thread_handle){0};
    Result = true;
  }
  else
  {
    u32 Error = GetLastError();
    Result = false;
    InvalidCodePath;
  }
  
  return Result;
}

PLATFORM_INTERLOCKED_CMP_EXCHNG_32(Win32InterlockedCmpExchng32)
{
  LONG OriginalValueAtDestination = InterlockedCompareExchange((LONG volatile*)Destination,
                                                               (LONG)NewValue,
                                                               (LONG)OldValue);
  bool Success = (OriginalValueAtDestination == OldValue);
  if (SuccessOut) *SuccessOut = Success;
  
  return (s32)OriginalValueAtDestination;
}

PLATFORM_INTERLOCKED_CMP_EXCHNG_64(Win32InterlockedCmpExchng64)
{
  LONG OriginalValueAtDestination = InterlockedCompareExchange64((LONG64 volatile*)Destination,
                                                                 (LONG64)NewValue,
                                                                 (LONG64)OldValue);
  bool Success = (OriginalValueAtDestination == OldValue);
  if (SuccessOut) *SuccessOut = Success;
  
  return (s64)OriginalValueAtDestination;
}

internal gs_thread_manager 
CreatePlatformThreadManagerBuffered(u64 ThreadCountMax, gs_thread_handle* ThreadHandleTable, gs_thread_proc_ctx* ThreadCtxArray)
{
  gs_thread_manager Result = ThreadManagerCreate(ThreadCountMax,
                                                 ThreadHandleTable,
                                                 ThreadCtxArray,
                                                 Win32GetThreadId,
                                                 Win32GetThreadHandle,
                                                 Win32GetThreadStatus,
                                                 Win32ThreadResume,
                                                 Win32ThreadCreate,
                                                 Win32ThreadKill,
                                                 Win32InterlockedCmpExchng32,
                                                 Win32InterlockedCmpExchng64,
                                                 0);
  return Result;
}

internal gs_thread_manager
CreatePlatformThreadManager(u64 ThreadCountMax)
{
  u64 SizeNeeded = ThreadCountMax * (sizeof(gs_thread_handle) + sizeof(gs_thread_proc_ctx));
  u8* Data = (u8*)VirtualAlloc(NULL, SizeNeeded,
                               MEM_COMMIT | MEM_RESERVE,
                               PAGE_EXECUTE_READWRITE);
  gs_thread_handle* ThreadHandleTable = (gs_thread_handle*)Data;
  gs_thread_proc_ctx* ThreadCtxArray = (gs_thread_proc_ctx*)(Data + (sizeof(gs_thread_handle) * ThreadCountMax));
  
  return CreatePlatformThreadManagerBuffered(ThreadCountMax, ThreadHandleTable, ThreadCtxArray);
}

internal void 
CleanupPlatformThreadManager(gs_thread_manager* T)
{
  for (u64 t = 0; t < T->ThreadCountMax; t++)
  {
    gs_thread_handle SystemThreadHandle = T->ThreadHandleTable[t];
    if (ThreadHandleIsValid(SystemThreadHandle))
    {
      ThreadKill(T, &SystemThreadHandle, 0);
    }
  }
  
  *T = (gs_thread_manager){0};
}

#endif //GS_THREAD_WIN32_H
