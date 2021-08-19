#ifndef GS_PROFILER_C
#define GS_PROFILER_C

#ifndef GS_MEMORY_H
# error "gs_profiler.c relies on gs_memory.h"
#endif

struct gs_profiler
{
  gs_profiler_frame* Frames;
  u64 FramesCount;
  u64 CurrentFrameIndex;
  
  gs_memory_arena Arena;
  
  gs_thread_manager* ThreadManager;
  gs_time_handler TimeHandler;
};

internal gs_profiler_scope_array*
ProfilerGetArrayDestForScope(gs_profiler* Profiler)
{
  gs_profiler_scope_array* Result = 0;
  
  gs_profiler_frame* CurrentFrame = Profiler->Frames + Profiler->CurrentFrameIndex;
  
  gs_thread_manager* T = Profiler->ThreadManager;
  u64 ThreadSystemId = T->PThreadGetId(T->UserData);
  u64 ThreadId = 0;
  for (u64 i = 0; i < T->ThreadCountMax; i++)
  {
    if (T->ThreadHandleTable[i].SystemId == ThreadSystemId)
    {
      ThreadId= i;
      break;
    }
  }
  Assert(ThreadId < CurrentFrame->ThreadScopesCount);
  Result = CurrentFrame->ThreadScopes + ThreadId;
  
  return Result;
}

internal gs_profiler_scope
BeginScope_(gs_profiler* Profiler, char* FileName, u64 LineNumber, char* FunctionName, u64 Counter)
{
  if (!Profiler) return (gs_profiler_scope){0};
  
  gs_profiler_scope_array* Dest = ProfilerGetArrayDestForScope(Profiler);
  
  // NOTE(PS): If you hit this, just go adjust the value of ScopesCount field of ProfilerCreate
  Assert(Dest->ScopesUsed < Dest->ScopesCount - 1);
  gs_profiler_scope Result = {
    .ScopeId = {
      .ScopeIndex = ++Dest->ScopesUsed,
      .FileName = FileName, 
      .LineNumber = LineNumber,
      .FunctionName = FunctionName,
      .ScopeId = Counter,
    },
    
    .ParentScopeIndex = Dest->CurrentScopeIndex,
    .CyclesStart = TicksGetCurrent(Profiler->TimeHandler),
    
    // TODO(PS): Thread Id
    
    .Profiler = Profiler,
  };
  Dest->CurrentScopeIndex = Result.ScopeId.ScopeIndex;
  return Result;
}

internal void 
EndScope(gs_profiler_scope* Scope)
{
  if (Scope->Profiler == 0) return;
  
  Scope->CyclesEnd = TicksGetCurrent(Scope->Profiler->TimeHandler);
  
  gs_profiler_scope_array* Dest = ProfilerGetArrayDestForScope(Scope->Profiler);
  Assert(Scope->ScopeId.ScopeIndex < Dest->ScopesCount);
  Dest->Scopes[Scope->ScopeId.ScopeIndex] = *Scope;
  
  Dest->CurrentScopeIndex = Scope->ParentScopeIndex;
  Dest->HighestScopeId = Max(Dest->HighestScopeId, Scope->ScopeId.ScopeId);
}

internal gs_profiler
ProfilerCreate(gs_memory_arena A, u64 FramesCount, u64 ScopesCount, gs_thread_manager* ThreadManager, gs_time_handler TimeHandler)
{
  gs_profiler Result = {
    .Arena = A,
    .ThreadManager = ThreadManager,
    .TimeHandler = TimeHandler,
    .FramesCount = FramesCount,
    .Frames = PushArray(&A, gs_profiler_frame, FramesCount),
  };
  
  u64 ThreadCountMax = Max(1, ThreadManager->ThreadCountMax);
  
  gs_profiler_scope_array* Threads = PushArray(&Result.Arena, gs_profiler_scope_array, FramesCount * ThreadCountMax);
  gs_profiler_scope* Scopes = PushArray(&Result.Arena, gs_profiler_scope, FramesCount * ThreadCountMax * ScopesCount);
  
  for (u64 i = 0; i < Result.FramesCount; i++)
  {
    gs_profiler_frame* Frame = Result.Frames + i;
    Frame->ThreadScopesCount = ThreadCountMax;
    Frame->ThreadScopes = Threads + (i * ThreadCountMax);;
    for (u64 j = 0; j < Frame->ThreadScopesCount; j++)
    {
      Frame->ThreadScopes[j] = (gs_profiler_scope_array){
        .ScopesCount = ScopesCount,
        .Scopes = Scopes + ((i * ThreadCountMax) * (j * ScopesCount)),
      };
    }
  }
  
  return Result;
}

internal void
ProfilerCreateGlobal(gs_allocator A, u64 FramesCount, u64 ScopesCount, gs_thread_manager* ThreadManager, gs_time_handler TimeHandler)
{
  gs_memory_arena Arena = MemoryArenaCreate(4096, 8, A, 0, 0, (char*)"Profiler Arena");
  gs_profiler* Profiler = PushStruct(&Arena, gs_profiler);
  *Profiler = ProfilerCreate(Arena, FramesCount, ScopesCount, ThreadManager, TimeHandler);
  GlobalProfiler = Profiler;
}

internal void
ProfilerAdvanceFrame(gs_profiler* Profiler)
{
  u64 NextFrameIndex = Profiler->CurrentFrameIndex + 1;
  if (NextFrameIndex >= Profiler->FramesCount)
  {
    NextFrameIndex = 0;
  }
  
  // do this before setting CurrentFrameIndex in case there's another thread
  // running that tries to increment ScopesUsed before we've reset the frame
  gs_profiler_frame* Frame = Profiler->Frames + NextFrameIndex;
  u64 ThreadScopesCount = Frame->ThreadScopesCount;
  for (u64 i = 0; i < ThreadScopesCount; i++)
  {
    gs_profiler_scope_array* ThreadScope = Frame->ThreadScopes + i;
    ThreadScope->ScopesUsed = 0;
    ThreadScope->CurrentScopeIndex = 0;
    MemoryClearArray(ThreadScope->Scopes, gs_profiler_scope, ThreadScope->ScopesUsed);
  }
  
  // TODO(PS): Does this need to be an atomic swap?
  Profiler->CurrentFrameIndex = NextFrameIndex;
}

//
// Timeline
//

internal gs_profiler_timeline_thread*
ProfilerTimelineFindParent(gs_profiler_timeline_thread* Timeline, u64 ParentScopeIndex)
{
  gs_profiler_timeline_thread* Result = 0;
  
  // NOTE(PS): if Timeline is the scope we're looking for, return Timeline
  // otherwise, we do a depth first search of its children, then its successors
  if ((!Timeline->Scope && ParentScopeIndex == 0) ||
      (Timeline->Scope && Timeline->Scope->ScopeId.ScopeIndex == ParentScopeIndex))
  {
    Result = Timeline;
  }
  else
  {
    if (Timeline->Children)
    {
      Result = ProfilerTimelineFindParent(Timeline->Children, ParentScopeIndex);
    }
    
    if (!Result)
    {
      for (gs_profiler_timeline_thread* At = Timeline->Next; At != 0; At = At->Next)
      {
        if (At->Scope->ScopeId.ScopeIndex == ParentScopeIndex)
        {
          Result = At;
        } 
        else if (At->Children)
        {
          Result = ProfilerTimelineFindParent(At->Children, ParentScopeIndex);
        }
        
        if (Result) break;
      }
    }
  }
  
  return Result;
}

internal void
ProfilerTimelineInsertScope(gs_profiler_timeline_thread* Timeline, gs_profiler_timeline_thread* TEntry)
{
  gs_profiler_scope Scope = *TEntry->Scope;
  if (Timeline->Children)
  {
    gs_profiler_timeline_thread* Prev = 0;
    gs_profiler_timeline_thread* Next = 0;
    for (gs_profiler_timeline_thread* Child = Timeline->Children;
         Child != 0;
         Child = Child->Next)
    {
      Next = Child;
      
      gs_profiler_scope ChildScope = *Child->Scope;
      if (Scope.CyclesEnd <= ChildScope.CyclesStart)
      {
        break;
      }
      else if (ChildScope.CyclesEnd <= Scope.CyclesStart)
      {
        // Do nothing
      }
      else
      {
        // NOTE(PS): I'm not sure if this is invalid, it implies these scopes were
        // overlapping, which shouldn't happen. Either the timing is off, or, one of 
        // these scopes should be the child of the other
        InvalidCodePath;
      }
      
      Prev = Child;
      Next = 0; // zeroing this in case Child is the last and we're appending to the list
    }
    
    if (Next)
    {
      TEntry->Next = Next;
    }
    else
    {
      TEntry->Next = 0;
    }
    
    if (Prev) 
    {
      Prev->Next = TEntry;
    }
    else
    {
      Timeline->Children = TEntry;
    }
    
  }
  else
  {
    Timeline->Children = TEntry;
    TEntry->Next = 0;
  }
}

internal gs_profiler_timeline
ProfilerTimelineCreate(gs_profiler Profiler, u64 FrameIndex, gs_memory_arena* Arena)
{
  gs_profiler_timeline Result = {};
  
  Assert(FrameIndex < Profiler.FramesCount);
  gs_profiler_frame Frame = Profiler.Frames[FrameIndex];
  
  Result.ThreadTimelinesCount = Frame.ThreadScopesCount;
  Result.ThreadTimelines = PushArray(Arena, gs_profiler_timeline_thread, Result.ThreadTimelinesCount);
  
  for (u64 t = 0; t < Result.ThreadTimelinesCount; t++)
  {
    gs_profiler_scope_array ThreadScopes = Frame.ThreadScopes[t];
    gs_profiler_timeline_thread* ThreadTimeline = Result.ThreadTimelines + t;
    *ThreadTimeline = (gs_profiler_timeline_thread){0};
    for (u64 i = 1; i < ThreadScopes.ScopesUsed; i++)
    {
      gs_profiler_scope* Scope = ThreadScopes.Scopes + i;
      
      // Create the entry - we'll need it in all cases
      gs_profiler_timeline_thread* TEntry = PushStruct(Arena, gs_profiler_timeline_thread);
      *TEntry = (gs_profiler_timeline_thread){0};
      TEntry->Scope = Scope;
      
      gs_profiler_timeline_thread* ParentScope = ProfilerTimelineFindParent(ThreadTimeline, Scope->ParentScopeIndex);
      if (!ParentScope) ParentScope = ThreadTimeline;
      
      ProfilerTimelineInsertScope(ParentScope, TEntry);
    }
  }
  return Result;
}

//
// Statistics
//

internal gs_profiler_statistics
ProfilerStatisticsCreate(gs_profiler Profiler, u64 FrameIndex, gs_memory_arena* Arena)
{
  Assert(FrameIndex < Profiler.FramesCount);
  
  u64 HighestScopeId = 0;
  gs_profiler_frame Frame = Profiler.Frames[FrameIndex];
  for (u64 t = 0; t < Frame.ThreadScopesCount; t++)
  {
    gs_profiler_scope_array ThreadScopes = Frame.ThreadScopes[t];
    HighestScopeId = Max(HighestScopeId, ThreadScopes.HighestScopeId);
  }
  
  u64 ScopesNeeded = HighestScopeId + 1;
  Assert(ScopesNeeded != 0);
  gs_profiler_statistics Result = {
    .ScopeStatsCount = ScopesNeeded,
    .ScopeStats = PushArray(Arena, gs_profiler_scope_statistics, ScopesNeeded),
  };
  
  for (u64 i = 0; i < Result.ScopeStatsCount; i++)
  {
    gs_profiler_scope_statistics* ScopeStats = Result.ScopeStats + i;
    *ScopeStats = (gs_profiler_scope_statistics){0};
    
    ScopeStats->CyclesLongest = 0;
    ScopeStats->CyclesShortest = MaxR32; 
  }
  
  for (u64 t = 0; t < Frame.ThreadScopesCount; t++)
  {
    gs_profiler_scope_array Thread = Frame.ThreadScopes[t];
    for (u64 i = 1; i <= Thread.ScopesUsed; i++)
    {
      gs_profiler_scope Scope = Thread.Scopes[i];
      u64 ScopeCyclesTotal = Scope.CyclesEnd - Scope.CyclesStart;
      
      gs_profiler_scope_statistics* ScopeStats = Result.ScopeStats + Scope.ScopeId.ScopeId;
      ScopeStats->Id = Scope.ScopeId;
      ScopeStats->CallCount += 1;
      ScopeStats->CyclesLongest = Max(ScopeStats->CyclesLongest, ScopeCyclesTotal);
      ScopeStats->CyclesShortest = Min(ScopeStats->CyclesShortest, ScopeCyclesTotal);
      ScopeStats->CyclesTotal += ScopeCyclesTotal;
    }
  }
  
  for (u64 i = 0; i < Result.ScopeStatsCount; i++)
  {
    gs_profiler_scope_statistics* ScopeStats = Result.ScopeStats + i;
    if (ScopeStats->CallCount == 0) continue;
    
    ScopeStats->CyclesAverage = ScopeStats->CyclesTotal / ScopeStats->CallCount;
  }
  
  return Result;
}

#endif // GS_PROFILER_C