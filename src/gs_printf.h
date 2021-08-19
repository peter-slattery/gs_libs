/* date = May 15th 2021 3:17 pm */

#ifndef GS_PRINTF_H
#define GS_PRINTF_H

#ifndef Assert
# define Assert(c) 
#endif

#ifndef InvalidCodePath
# define InvalidCodePath
#endif

// TODO(PS): Context Logging
#define PRINTF_ERROR InvalidCodePath

enum {
  PrintMode_Start,
  PrintMode_OpenBrace,
  PrintMode_CloseBrace,
} gs_printf_mode_;

typedef struct gs_printf_state
{
  char* Dest;
  u64 DestLength;
  u64 LengthPrinted;
  u64 LengthNeeded;
  
  char* Format;
  u64 FormatLength;
  u64 FormatAt;
  
  u8 Mode;
  u64 CaptureStart;
} gs_printf_state;

#define PRINTF_TYPE_PRINT(name) void name(gs_printf_state* PFState, void* Value)
typedef PRINTF_TYPE_PRINT(gs_printf_type_print);

//
// A dictionary that associates a key = the hash of the type name
// with the procedure to print that type.
// collisions are dealt with by moving to the next free index
typedef struct gs_printf_types
{
  u64 FormatsMax;
  u64* FormatHashes;
  gs_printf_type_print** PrintProcs;
} gs_printf_types;

global gs_printf_types GlobalPrintFTypes;

internal u64
PrintFHash(char* Str, u64 Len)
{
  u64 Hash = 5381;
  for (u32 i = 0; i < Len; i++)
  {
    Hash = ((Hash << 5) + Hash) + Str[i];
  }
  return Hash;
}

internal s64
PrintFTypeHashLookup(u64 StartIndex, u64 TargetHash)
{
  s64 Result = -1;
  s64 Index = StartIndex;
  if (GlobalPrintFTypes.FormatHashes[Index] != TargetHash)
  {
    Index += 1;
    while (GlobalPrintFTypes.FormatHashes[Index] != TargetHash &&
           Index != StartIndex)
    {
      Index = (Index + 1) % GlobalPrintFTypes.FormatsMax;
    }
  }
  
  if (GlobalPrintFTypes.FormatHashes[Index] == TargetHash)
  {
    Result = Index;
  }
  
  return Result;
}

internal void
PrintFRegisterTypeL(char* Format, u64 FormatLen, gs_printf_type_print* Proc)
{
  u64 FormatHash = PrintFHash(Format, FormatLen);
  u64 StartIndex = FormatHash % GlobalPrintFTypes.FormatsMax;
  
  // NOTE(PS): Ensures this format hash isn't already present
  Assert(PrintFTypeHashLookup(StartIndex, FormatHash) == -1);
  
  s64 Index = PrintFTypeHashLookup(StartIndex, 0);
  
  // NOTE(PS): This isn't actually an error, it just means you
  // need to grow the size of GlobalPrintFTypes.FormatsMax
  Assert(GlobalPrintFTypes.FormatHashes[Index] == 0);
  
  GlobalPrintFTypes.FormatHashes[Index] = FormatHash;
  GlobalPrintFTypes.PrintProcs[Index] = Proc;
}

internal void
PrintFRegisterType(char* Format, gs_printf_type_print* Proc)
{
  u64 FormatLen =  CStringLength(Format);
  PrintFRegisterTypeL(Format, FormatLen, Proc);
}

typedef struct gs_printf_type
{
  gs_printf_type_print* Proc;
} gs_printf_type;

internal gs_printf_type
PrintFGetPrintTypeProc(char* Format, u64 FormatLen)
{
  u64 FormatHash = PrintFHash(Format, FormatLen);
  u64 StartIndex = FormatHash % GlobalPrintFTypes.FormatsMax;
  u64 Index = PrintFTypeHashLookup(StartIndex, FormatHash);
  
  // NOTE(PS): This means that you didn't register this
  // type as a printable format
  Assert(GlobalPrintFTypes.FormatHashes[Index] != 0);
  
  gs_printf_type Result = {
    .Proc = GlobalPrintFTypes.PrintProcs[Index],
  };
  return Result;
}

internal void
PrintFWrite(gs_printf_state* PFState, char C)
{
  u64 Index = PFState->LengthNeeded++;
  if (Index < PFState->DestLength)
  {
    PFState->Dest[Index] = C;
    PFState->LengthPrinted++;
  }
}

internal void
PrintFWriteRange(gs_printf_state* PFState, char* Str, u64 First, u64 OnePastLast)
{
  for (u64 i = First; i < OnePastLast; i++)
  {
    PrintFWrite(PFState, Str[i]);
  }
}

internal void
SPrintFVarArgs_(gs_printf_state* PFState, va_list Args)
{
  u64 FormatLen = PFState->FormatLength;
  for (u64 i = 0; i < FormatLen; i++)
  {
    char At = PFState->Format[i];
    switch (PFState->Mode)
    {
      case PrintMode_Start:
      {
        if (At == '{')
        {
          PrintFWriteRange(PFState, PFState->Format, PFState->CaptureStart, i);
          PFState->Mode = PrintMode_OpenBrace;
          PFState->CaptureStart = i + 1;
        }
        else if (At == '}')
        {
          PrintFWriteRange(PFState, PFState->Format, PFState->CaptureStart, i);
          PFState->Mode = PrintMode_CloseBrace;
        }
        else {}
      } break;
      
      case PrintMode_OpenBrace:
      {
        if (At == '{')
        {
          PFState->Mode = PrintMode_Start;
          PFState->CaptureStart = i;
        }
        else if (At == '}')
        {
          char* ArgTypeStr = PFState->Format + PFState->CaptureStart;
          u64 ArgTypeStrLen = i - PFState->CaptureStart;
          gs_printf_type Type = PrintFGetPrintTypeProc(ArgTypeStr, ArgTypeStrLen);
          
          void* NextArg = va_arg(Args, void*);
          
          Type.Proc(PFState, NextArg);
          
          PFState->Mode = PrintMode_Start;
          PFState->CaptureStart = i + 1;
        }
        else 
        {
        }
      } break;
      
      case PrintMode_CloseBrace:
      {
        if (At == '}')
        {
          PFState->Mode = PrintMode_Start;
          PFState->CaptureStart = i;
        }
        else
        {
          PRINTF_ERROR;
        }
      } break;
    }
  }
  
  if (PFState->CaptureStart < PFState->FormatLength)
  {
    PrintFWriteRange(PFState, PFState->Format, PFState->CaptureStart, PFState->FormatLength);
  }
}

internal u64
SPrintFVarArgs(char* Dest, u64 DestLen, char* Format, va_list Args)
{
  gs_printf_state PFState = {
    .Dest = Dest,
    .DestLength = DestLen,
    .Format = Format,
    .FormatLength = CStringLength(Format),
    .Mode = PrintMode_Start
  };
  
  SPrintFVarArgs_(&PFState, Args);
  
  return PFState.LengthNeeded;
}

internal void
SPrintFNested(gs_printf_state* PFState, char* NewFormat, ...)
{
  gs_printf_state NewPFState = {
    .Dest = PFState->Dest,
    .DestLength = PFState->DestLength,
    .LengthPrinted = PFState->LengthPrinted,
    .LengthNeeded = PFState->LengthNeeded,
    
    .Format = NewFormat,
    .FormatLength = CStringLength(NewFormat),
    .Mode = PrintMode_Start
  };
  
  va_list Args;
  va_start(Args, NewFormat);
  SPrintFVarArgs_(&NewPFState, Args);
  va_end(Args);
  
  PFState->LengthPrinted = NewPFState.LengthPrinted;
  PFState->LengthNeeded = NewPFState.LengthNeeded;
}

internal u64
SPrintF(char* Dest, u64 DestLen, char* Format, ...)
{
  va_list Args;
  va_start(Args, Format);
  u64 Result = SPrintFVarArgs(Dest, DestLen, Format, Args);
  va_end(Args);
  return Result;
}

// TODO(PS): PrintF - where it allocates using a #defined Allocator, tries to print, gets the length it needs, and prints again
// TODO(PS): a #ifdef USE_CONTEXT that lets printf default to using a the gs_thread_context struct

//
// Basic Print Procs
//

internal void
PrintFReverseStringInPlace(char* Str, u64 Len)
{
  if (Str)
  {
    char* Start = Str;
    char* End = Str + Len;
    while (Start < End)
    {
      End--;
      char Temp = End[0];
      End[0] = Start[0];
      Start[0] = Temp;
      Start++;
    }
  }
}

PRINTF_TYPE_PRINT(PrintF_CString)
{
  char* StrValue = *(char**)Value;
  for (char* At = StrValue; *At != 0; At++)
  {
    PrintFWrite(PFState, *At);
  }
}

PRINTF_TYPE_PRINT(PrintF_UnsignedInt)
{
  u64 Base = 10;
  
  u64 StartChar = PFState->LengthPrinted;
  
  u64 UIntValue = *((u64*)Value);
  
  u64 ValueRemaining = UIntValue;
  do {
    u64 PlaceValue = ValueRemaining % Base;
    char Digit = '0' + PlaceValue;
    PrintFWrite(PFState, Digit);
    ValueRemaining /= Base;
  } while(ValueRemaining != 0);
  
  u64 EndChar = PFState->LengthPrinted;
  u64 CharsWritten = EndChar - StartChar;
  PrintFReverseStringInPlace(PFState->Dest + StartChar, CharsWritten);
}

PRINTF_TYPE_PRINT(PrintF_SignedInt)
{
  u64 LengthWritten = 0;
  
  s64 SIntValue = *((s64*)Value);
  if (SIntValue < 0)
  {
    PrintFWrite(PFState, '-');
    SIntValue = Abs(SIntValue);
  }
  
  PrintF_UnsignedInt(PFState, (u64*)(&SIntValue));
}

PRINTF_TYPE_PRINT(PrintF_S8)
{
  s8 V = *((s8*)Value);
  s64 SIntValue = (s64)V;
  PrintF_SignedInt(PFState, (s64*)(&SIntValue));
}

PRINTF_TYPE_PRINT(PrintF_S16)
{
  s16 V = *((s16*)Value);
  s64 SIntValue = (s64)V;
  PrintF_SignedInt(PFState, (s64*)(&SIntValue));
}

PRINTF_TYPE_PRINT(PrintF_S32)
{
  s32 V = *((s32*)Value);
  s64 SIntValue = (s64)V;
  PrintF_SignedInt(PFState, (s64*)(&SIntValue));
}

PRINTF_TYPE_PRINT(PrintF_U8)
{
  u8 V = *((u8*)Value);
  u64 UIntValue = (u64)V;
  PrintF_UnsignedInt(PFState, (u64*)(&UIntValue));
}

PRINTF_TYPE_PRINT(PrintF_U16)
{
  u16 V = *((u16*)Value);
  u64 UIntValue = (u64)V;
  PrintF_UnsignedInt(PFState, (u64*)(&UIntValue));
}

PRINTF_TYPE_PRINT(PrintF_U32)
{
  u32 V = *((u32*)Value);
  u64 SIntValue = (u64)V;
  PrintF_UnsignedInt(PFState, (u64*)(&SIntValue));
}

PRINTF_TYPE_PRINT(PrintF_B64)
{
  b64 BoolValue = *((b64*)Value);
  char* ValueString = (char*)(BoolValue ? "true" : "false");
  PrintF_CString(PFState, &ValueString);
}

PRINTF_TYPE_PRINT(PrintF_Bool)
{
  bool V = *((bool*)Value);
  b64 B64Value = (b64)V;
  PrintF_B64(PFState, (b64*)(&B64Value));
}

PRINTF_TYPE_PRINT(PrintF_B8)
{
  b8 V = *((b8*)Value);
  b64 B64Value = (b64)V;
  PrintF_B64(PFState, (b64*)(&B64Value));
}

PRINTF_TYPE_PRINT(PrintF_B32)
{
  b32 V = *((b32*)Value);
  b64 B64Value = (b64)V;
  PrintF_B64(PFState, (b64*)(&B64Value));
}

PRINTF_TYPE_PRINT(PrintF_R64)
{
  r64 FloatValue = *((r64*)Value);
  if (FloatValue < 0)
  {
    PrintFWrite(PFState, '-');
    FloatValue = Abs(FloatValue);
  }
  u64 IntegerPart = (u64)FloatValue;
  PrintF_UnsignedInt(PFState, (void*)&IntegerPart);
  
  FloatValue -= IntegerPart;
  
  // TODO(PS): make this a setting for PrintF?
  // have to figure out a way to make that thread safe
  // although we probably don't want to be printing from multiple threads...
  u64 Precision = 6; 
  if (FloatValue > 0)
  {
    PrintFWrite(PFState, '.');
    for (u64 i = 0; i < Precision; i++)
    {
      FloatValue *= 10.0f;
      u64 DecimalPlace = (u64)FloatValue;
      char DecimalChar = '0' + DecimalPlace;
      PrintFWrite(PFState, DecimalChar);
      FloatValue -= DecimalPlace;
    }
  }
}

PRINTF_TYPE_PRINT(PrintF_R32)
{
  r32 V = *((r32*)Value);
  r64 R64Value = (r64)V;
  PrintF_R64(PFState, (r64*)(&R64Value));
}

#ifdef GS_PROFILER_H

internal void
PrintF_ProfilerTimelineIndent(gs_printf_state* PFState, s32 Indent)
{
  for (s32 i = 0; i < Indent; i++)
  {
    PrintFWrite(PFState, '\t');
  }
}

internal void
PrintF_ProfilerTimelineThread(gs_printf_state* PFState, gs_profiler_timeline_thread* Thread, s32 Indent)
{
  char* FunctionName = (char*)"<root>";
  if (Thread->Scope) { FunctionName = Thread->Scope->ScopeId.FunctionName; }
  
  PrintF_ProfilerTimelineIndent(PFState, Indent);
  SPrintFNested(PFState, (char*)">>> {char*}\n", &FunctionName);
  {
    for (gs_profiler_timeline_thread* Child = Thread->Children;
         Child != 0;
         Child = Child->Next)
    {
      PrintF_ProfilerTimelineThread(PFState, Child, Indent + 1);
    }
  }
  PrintF_ProfilerTimelineIndent(PFState, Indent);
  SPrintFNested(PFState, (char*)"<<< {char*}\n", &FunctionName);
}

PRINTF_TYPE_PRINT(PrintF_ProfilerTimeline)
{
  gs_profiler_timeline* T = (gs_profiler_timeline*)Value;
  
  s32 Indent = 0;
  for (u64 t = 0; t < T->ThreadTimelinesCount; t++)
  {
    SPrintFNested(PFState, (char*)"THREAD: {u64}\n", &t);
    PrintF_ProfilerTimelineThread(PFState, T->ThreadTimelines + t, Indent);
  }
}

#endif

internal void
PrintFRegisterBasics()
{
  PrintFRegisterType((char*)"char*", PrintF_CString);
  
  PrintFRegisterType((char*)"u8", PrintF_U8);
  PrintFRegisterType((char*)"u16", PrintF_U16);
  PrintFRegisterType((char*)"u32", PrintF_U32);
  PrintFRegisterType((char*)"u64", PrintF_UnsignedInt);
  
  PrintFRegisterType((char*)"s8", PrintF_S8);
  PrintFRegisterType((char*)"s16", PrintF_S16);
  PrintFRegisterType((char*)"s32", PrintF_S32);
  PrintFRegisterType((char*)"s64", PrintF_SignedInt);
  
  PrintFRegisterType((char*)"bool", PrintF_Bool);
  PrintFRegisterType((char*)"b8", PrintF_B8);
  PrintFRegisterType((char*)"b32", PrintF_B32);
  PrintFRegisterType((char*)"b64", PrintF_B64);
  
  PrintFRegisterType((char*)"r32", PrintF_R32);
  PrintFRegisterType((char*)"r64", PrintF_R64);
  
#ifdef GS_PROFILER_H
  PrintFRegisterType((char*)"gs_profiler_timeline", PrintF_ProfilerTimeline);
#endif
}

//
// Initialization
//

internal void
PrintFReset()
{
  MemoryClearArray(GlobalPrintFTypes.FormatHashes, u64, GlobalPrintFTypes.FormatsMax);
}

internal void
PrintFInitNoBasic(gs_allocator A)
{
  u64 FormatsMax = 1024;
  GlobalPrintFTypes.FormatsMax = FormatsMax;
  GlobalPrintFTypes.FormatHashes = AllocArray(A, u64, FormatsMax, "root");
  GlobalPrintFTypes.PrintProcs = AllocArray(A, gs_printf_type_print*, FormatsMax, "root");
  
  PrintFReset();
}

internal void
PrintFInit(gs_allocator A)
{
  PrintFInitNoBasic(A);
  PrintFRegisterBasics();
}

//
// Special variants that rely on other gs_libs
//

#ifdef GS_MEMORY_H

typedef struct gs_mprintfv_result
{
  char* Str;
  u64 Length;
} gs_mprintfv_result;

internal gs_mprintfv_result
MPrintFVVarArgs(gs_memory_arena* A, char* Format, va_list Args0, bool NullTerminate)
{
  gs_mprintfv_result Result = { .Str = 0, .Length = 0 };
  
  va_list Args1;
  va_copy(Args1, Args0);
  
  // run sprintf on an empty string to figure out how long a string
  // we need
  Result.Length = SPrintFVarArgs(Result.Str, 0, Format, Args0);
  
  u64 AllocLength = Result.Length;
  if (NullTerminate) AllocLength += 1;
  
  // allocate the needed string length and sprintf into it
  Result.Str = PushArray(A, char, AllocLength);
  u64 LengthNeeded = SPrintFVarArgs(Result.Str, Result.Length, Format, Args1);
  Assert(LengthNeeded == Result.Length);
  
  if (NullTerminate)
  {
    Result.Str[Result.Length] = 0;
  }
  
  va_end(Args1);
  return Result;
}

internal gs_mprintfv_result
MPrintFV(gs_memory_arena* A, char* Format, ...)
{
  va_list Args;
  va_start(Args, Format);
  gs_mprintfv_result Result = MPrintFVVarArgs(A, Format, Args, false);
  va_end(Args);
  return Result;
}

// MPrintF is for when you don't care about the length - but it makes sure
// to null terminate the end
internal char*
MPrintF(gs_memory_arena* A, char* Format, ...)
{
  va_list Args;
  va_start(Args, Format);
  gs_mprintfv_result Result = MPrintFVVarArgs(A, Format, Args, true);
  va_end(Args);
  return Result.Str;
}

#endif // GS_MEMORY_H

#endif //GS_PRINTF_H
