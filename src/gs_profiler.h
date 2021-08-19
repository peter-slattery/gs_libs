/* date = May 29th 2021 2:19 pm */
#ifndef GS_PROFILER_H
#define GS_PROFILER_H

typedef struct gs_profiler gs_profiler;

typedef struct gs_profiler_scope_id
{
  u64 ScopeIndex;
  char* FileName;
  u64 LineNumber;
  char* FunctionName;
  u64 ScopeId;
} gs_profiler_scope_id;

typedef struct gs_profiler_scope
{
  gs_profiler_scope_id ScopeId;
  u64 ParentScopeIndex;
  
  u64 CyclesStart;
  u64 CyclesEnd;
  u64 ThreadId;
  
  gs_profiler* Profiler;
} gs_profiler_scope;

typedef struct gs_profiler_scope_array
{
  gs_profiler_scope* Scopes;
  u64 ScopesCount;
  u64 ScopesUsed;
  
  u64 CurrentScopeIndex;
  u64 HighestScopeId;
} gs_profiler_scope_array;

typedef struct gs_profiler_frame
{
  gs_profiler_scope_array* ThreadScopes;
  u64 ThreadScopesCount;
} gs_profiler_frame;

global gs_profiler* GlobalProfiler;

#define BeginScope(profiler, counter) BeginScope_((profiler), (char*)__FILE__, (u64)__LINE__, (char*)__FUNCTION__, (counter))
internal gs_profiler_scope BeginScope_(gs_profiler* Profiler, char* FileName, u64 LineNumber, char* FunctionName, u64 Counter);
internal void EndScope(gs_profiler_scope* Scope);

// TODO(PS): do a version that uses the cpp destructor method?

#define PROFILE_SCOPE_NAME_(l) Scope##l
#define PROFILE_SCOPE_NAME(l) PROFILE_SCOPE_NAME_(l)
#define PROFILE_SCOPE(profiler) __attribute__((cleanup(EndScope))) gs_profiler_scope PROFILE_SCOPE_NAME(__LINE__) = BeginScope((profiler), __COUNTER__)
#define PROFILE_SCOPE_G PROFILE_SCOPE(GlobalProfiler)

//
// Timeline
//

typedef struct gs_profiler_timeline_thread
{
  gs_profiler_scope* Scope;
  struct gs_profiler_timeline_thread* Children;
  struct gs_profiler_timeline_thread* Next;
} gs_profiler_timeline_thread;

typedef struct gs_profiler_timeline
{
  u64 ThreadTimelinesCount;
  gs_profiler_timeline_thread* ThreadTimelines;
} gs_profiler_timeline;

//
// Statistics
//

typedef struct gs_profiler_scope_statistics
{
  gs_profiler_scope_id Id;
  u64 CallCount;
  u64 CyclesLongest;
  u64 CyclesShortest;
  u64 CyclesAverage;
  u64 CyclesTotal;
} gs_profiler_scope_statistics;

typedef struct gs_profiler_statistics
{
  u64 ScopeStatsCount;
  gs_profiler_scope_statistics* ScopeStats;
} gs_profiler_statistics;

#endif //GS_PROFILER_H
