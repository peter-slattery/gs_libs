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
#include "../src/memory/gs_memory.h"
#include "../src/file/gs_file.h"
#include "../src/thread/gs_thread.h"
#include "../src/time/gs_time.h"
#include "../src/gs_debug_break.h"
#include "../src/gs_math.h"
#include "../src/gs_printf.h"

#include "../src/gs_profiler.h"
#include "../src/gs_profiler.c"
internal u32
TestFunctionTwo (u32 In)
{
  PROFILE_SCOPE_G;
  
  u32 x = 5;
  for (u32 i = 0; i < 10000; i++)
  {
    x += i * In;
  }
  
  return x;
}

internal u32
TestFunctionOne (u32 In)
{
  PROFILE_SCOPE_G;
  
  u32 x = 5;
  for (u32 i = 0; i < In; i++)
  {
    TestFunctionTwo(i);
  }
  
  return x;
}

internal void
MainTest ()
{
  PROFILE_SCOPE_G;
  
  u32 Result = 0;
  Result += TestFunctionOne(3);
  Result += TestFunctionOne(5);
  Result += TestFunctionOne(16);
}

internal s32
ThreadTest(gs_thread_proc_ctx* Ctx)
{
  PROFILE_SCOPE_G;
  
  u32 Result = 0;
  Result += TestFunctionOne(3);
  Result += TestFunctionOne(5);
  Result += TestFunctionOne(16);
  
  return 0;
}

internal void
ProfilerTimelineIndent(s32 Indent)
{
  for (s32 i = 0; i < Indent; i++)
  {
    printf("\t");
  }
}

internal void
ProfilerTimelineThreadPrint(gs_profiler_timeline_thread* T, s32 Indent)
{
  ProfilerTimelineIndent(Indent);
  char* FunctionName = "<root>";
  if (T->Scope) { FunctionName = T->Scope->ScopeId.FunctionName; }
  printf(">>> %s\n", FunctionName);
  {
    for (gs_profiler_timeline_thread* Child = T->Children;
         Child != 0;
         Child = Child->Next)
    {
      ProfilerTimelineThreadPrint(Child, Indent + 1);
    }
  }
  ProfilerTimelineIndent(Indent);
  printf("<<< %s\n", FunctionName);
}

internal void
ProfilerTimelinePrint(gs_profiler_timeline* T, s32 Indent)
{
  printf("==== FRAME START ====\n");
  for (u64 t = 0; t < T->ThreadTimelinesCount; t++)
  {
    printf("THREAD: %lld\n", t);
    ProfilerTimelineThreadPrint(T->ThreadTimelines + t, Indent + 1);
    printf("\n\n");
  }
}

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  
  gs_allocator Allocator = CreatePlatformAllocator();
  gs_memory_arena T = MemoryArenaCreate(4096, 8, Allocator, 0, 0, "Transient");
  
  gs_thread_manager ThreadManager = CreatePlatformThreadManager(2);
  gs_time_handler TimeHandler = CreatePlatformTimeHandler();
  
  ProfilerCreateGlobal(Allocator, 16, 4096, &ThreadManager, TimeHandler);
  
  TestGroup("Profiler")
  {
    for (u32 i = 0; i < 32; i++)
    {
      ProfilerAdvanceFrame(GlobalProfiler);
      
      gs_thread_handle THandle = ThreadCreate(&ThreadManager, 4096, ThreadTest, 0, 0);
      MainTest();
      
      while (ThreadGetStatus(&ThreadManager, &THandle) == ThreadStatus_Active)
      {
        Sleep(1);
      }
      
      gs_profiler_frame Frame = GlobalProfiler->Frames[GlobalProfiler->CurrentFrameIndex];
      gs_profiler_timeline Timeline = ProfilerTimelineCreate(*GlobalProfiler, GlobalProfiler->CurrentFrameIndex, &T);
      gs_profiler_statistics Stats = ProfilerStatisticsCreate(*GlobalProfiler, GlobalProfiler->CurrentFrameIndex, &T);
      
      // print out the results
#if 0
      printf("Profiler Results\n");
      for (u64 t = 0; t < Frame.ThreadScopesCount; t++)
      {
        gs_profiler_scope_array Thread = Frame.ThreadScopes[t];
        for (u64 i = 1; i < Thread.ScopesUsed; i++)
        {
          gs_profiler_scope Scope = Thread.Scopes[i];
          char* ParentFunction = "<root>";
          if (Scope.ParentScopeIndex != 0)
          {
            gs_profiler_scope Parent = Thread.Scopes[Scope.ParentScopeIndex];
            ParentFunction = Parent.ScopeId.FunctionName;
          }
          printf("\t%s - %lld\n\t\t%s\n", Scope.ScopeId.FunctionName, (Scope.CyclesEnd - Scope.CyclesStart), ParentFunction);
        }
      }
      
      printf("\n\n");
      
      printf("Profiler Timeline\n");
      ProfilerTimelinePrint(&Timeline, 0);
      
      printf("\n\n");
      
      printf("Profiler Stats\n");
      
      for (u64 i = 0; i < Stats.ScopeStatsCount; i++)
      {
        gs_profiler_scope_statistics S = Stats.ScopeStats[i];
        printf("\t%s - %lld calls - longest: %lld cy - shortest: %lld cy - avg: %lld cy - total: %lld cy\n",
               S.Id.FunctionName, S.CallCount, S.CyclesLongest, S.CyclesShortest, S.CyclesAverage, S.CyclesTotal);
      }
#endif
      
    }
    
    // NOTE(PS): basically, if we get this far, we should be good
    GlobalTest(true);
  }
  
  return (0);
}