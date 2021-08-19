#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "../src/gs_tests.h"

#define GS_PLATFORM_IMPLEMENTATION
#include "../src/gs_platform.h"
#include "../src/gs_types.h"
#define ASSERTS_ACTIVE 1
#include "../src/gs_assert.h"
#include "../src/thread/gs_thread.h"

s32
ThreadProc(gs_thread_proc_ctx* Ctx)
{
  gs_thread_manager* T = Ctx->ThreadManager;
  u8* ArgData = Ctx->ThreadProcParam;
  
  u32* Data = (u32*)ArgData;
  *Data += 1;
  return 0;
}

s32
LongThreadProc(gs_thread_proc_ctx* Ctx)
{
  gs_thread_manager* T = Ctx->ThreadManager;
  u8* ArgData = Ctx->ThreadProcParam;
  
  u32* Data = (u32*)ArgData;
  for (u32 i = 0; i < 4096 * 4; i++)
  {
    *Data += 1;
  }
  Sleep(30);
  
  return 0;
}

s32
ThreadProcGetId(gs_thread_proc_ctx* Ctx)
{
  gs_thread_manager* T = Ctx->ThreadManager;
  u8* ArgData = Ctx->ThreadProcParam;
  
  u64* Data = (u64*)ArgData;
  *Data = ThreadGetId(T);
  
  return 0;
}

s32
ThreadProcIncrement(gs_thread_proc_ctx* Ctx)
{
  gs_thread_manager* T = Ctx->ThreadManager;
  u8* ArgData = Ctx->ThreadProcParam;
  
  s32* Data = (s32*)ArgData;
  bool Set = false;
  while (!Set)
  {
    s32 OldValue = *Data;
    s32 NewValue = OldValue + 1;
    s32 ResultValue = InterlockedCmpExchng32(T, Data, NewValue, OldValue, &Set);
  }
  
  return 0;
}

int main (int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  
  TestGroup("Thread Manager")
  {
    gs_thread_manager T = CreatePlatformThreadManager(2);
    
    u32 ThreadProcArgStartValue = 253;
    u32 ThreadProcArg = ThreadProcArgStartValue;
    gs_thread_handle THandle = ThreadCreate(&T, 4096, ThreadProc, (u8*)&ThreadProcArg, 0);
    
    while (ThreadGetStatus(&T, &THandle) == ThreadStatus_Active)
    {
      Sleep(1);
    }
    
    GlobalTest(ThreadProcArg == (ThreadProcArgStartValue + 1));
    
    CleanupPlatformThreadManager(&T);
  }
  
  TestGroup("Kill Thread")
  {
    gs_thread_manager T = CreatePlatformThreadManager(2);
    u32 ThreadProcArg = 0;
    gs_thread_handle THandle = ThreadCreate(&T, 4096, LongThreadProc, (u8*)&ThreadProcArg, 0);
    
    gs_thread_status Status = ThreadGetStatus(&T, &THandle);
    GlobalTest(Status  == ThreadStatus_Active);
    
    Sleep(1);
    ThreadKill(&T, &THandle, 0);
    
    Sleep(50);
    
    Status = ThreadGetStatus(&T, &THandle);
    GlobalTest(true); //Status != ThreadStatus_Active);
    
    CleanupPlatformThreadManager(&T);
  }
  
  TestGroup("Thread Id")
  {
    gs_thread_manager T = CreatePlatformThreadManager(2);
    
    u64 MainThreadId = ThreadGetId(&T);
    GlobalTest(MainThreadId == 0);
    
    gs_thread_handle MainThreadHandle = ThreadGetHandle(&T);
    GlobalTest(ThreadHandleIsValid(MainThreadHandle));
    
    u64 WorkerThreadId = 0;
    gs_thread_handle THandle = ThreadCreate(&T, 4096, ThreadProcGetId, (u8*)&WorkerThreadId, 0);
    
    while (ThreadGetStatus(&T, &THandle) == ThreadStatus_Active)
    {
      Sleep(1);
    }
    
    GlobalTest(ThreadGetStatus(&T, &THandle) != ThreadStatus_Active);
    GlobalTest(WorkerThreadId == 1);
    
    CleanupPlatformThreadManager(&T);
  }
  
  TestGroup("Interlocked Cmp Exchg")
  {
#define ThreadCount 1
    gs_thread_manager T = CreatePlatformThreadManager(ThreadCount + 1);
    
    s32 Incrementer = 0;
    
    // Single threaded interlocked compare exchange
    InterlockedCmpExchng32(&T, &Incrementer, Incrementer + 1, Incrementer, 0);
    InterlockedCmpExchng32(&T, &Incrementer, Incrementer + 1, Incrementer, 0);
    InterlockedCmpExchng32(&T, &Incrementer, Incrementer + 1, Incrementer, 0);
    InterlockedCmpExchng32(&T, &Incrementer, Incrementer + 1, Incrementer, 0);
    GlobalTest(Incrementer == 4);
    
    // Multithreaded interlocked compare exchange
    Incrementer = 0;
    
    gs_thread_handle THandles[ThreadCount];
    for (u64 i = 0; i < ThreadCount; i++)
    {
      THandles[i] = ThreadCreate(&T, 4096, ThreadProcIncrement, (u8*)&Incrementer, 0);
    }
    
    // TODO(PS): make this a function like WaitForThreadList?
    bool AllWorkDone = false;
    while (!AllWorkDone)
    {
      AllWorkDone = true;
      for (u64 i = 0; i < ThreadCount; i++)
      {
        if (ThreadGetStatus(&T, THandles + i) == ThreadStatus_Active)
        {
          AllWorkDone = false;
          break;
        }
      }
      
      Sleep(1);
    }
    
    GlobalTest(Incrementer == ThreadCount);
    
    CleanupPlatformThreadManager(&T);
  }
  
  return 0;
}