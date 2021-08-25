/* date = August 16th 2021 9:04 pm */

#ifndef GS_STRING_H
#define GS_STRING_H

typedef struct {
  u8* Data;
  
  // the current length of the string
  u64 Len;
  
  // the maximum length of the string (ie size of Data)
  u64 Cap; 
} gs_string;

internal gs_string
StringCreate(u8* Data, u64 Len, u64 Cap)
{
  gs_string Result = {};
  Result.Data = Data;
  Result.Len = Len;
  Result.Cap = Cap;
  return Result;
}

internal gs_string
StringCreateConst(u8* Data, u64 Len)
{
  return StringCreate(Data, Len, Len);
}

// NOTE(PS): creates a gs_string out of a string literal,
// Len and Cap will not include the null terminator
#define LitStr(s) (gs_string){ (u8*)(s), (sizeof(s)/sizeof(s[0])) - 1, (sizeof(s)/sizeof(s[0])) - 1 }

// Index Of - Fwd / Bwd

internal s64
StringIndexOfCharFwd(gs_string Str, char C)
{
  for (u64 i = 0; i < Str.Len; i++) 
  {
    if (Str.Data[i] == C) return (s64)i;
  }
  return -1;
}

internal s64
StringIndexOfCharBwd(gs_string Str, char C)
{
  for (u64 i = Str.Len; i > 0; i--) 
  {
    s64 LookupIndex = (s64)(i-1);
    if (Str.Data[LookupIndex] == C) return LookupIndex;
  }
  return -1;
}

internal s64
StringIndexOfStringFwd(gs_string Str, gs_string Needle)
{
  for (u64 i = 0; i < Str.Len; i++) 
  {
    if (Str.Data[i] == Needle.Data[0]) 
    {
      bool AllMatch = true;
      u64 StrRoomLeft = Str.Len - i;
      u64 MatchIters = Min(Needle.Len, StrRoomLeft);
      for (u64 j = 1; j < MatchIters; j++) 
      {
        if (Str.Data[i + j] != Needle.Data[j]) 
        {
          AllMatch = false;
          break;
        }
      }
      if (AllMatch) return i;
    }
  }
  return -1;
}

internal s64
StringIndexOfStringBwd(gs_string Str, gs_string Needle)
{
  u8* StrAt = Str.Data + (Str.Len - 1);
  u8* NeedleStart = Needle.Data + (Needle.Len - 1);
  for (; (StrAt - Str.Data) > 0; StrAt--) 
  {
    if (*StrAt == *NeedleStart) 
    {
      u8* NeedleAt = NeedleStart - 1;
      u8* StrResumePoint = StrAt--;
      bool AllMatch = true;
      for (; 
           (NeedleAt - Needle.Data) > 0 && (StrAt - Str.Data) > 0;
           NeedleAt--, StrAt--) 
      {
        if (*StrAt != *NeedleAt) 
        {
          AllMatch = false;
          break;
        }
      }
      if (AllMatch) return (s64)(StrAt - Str.Data);
      
      StrAt = StrResumePoint;
    }
  }
  return -1;
}

//
// Contains
//

internal bool
StringContainsChar(gs_string Str, char C) 
{
  return StringIndexOfCharFwd(Str, C) >= 0;
}

internal bool
StringContainsString(gs_string Str, gs_string Needle)
{
  return StringIndexOfStringFwd(Str, Needle) >= 0;
}

//
// Substring
//

// NOTE(PS): if Start or End are negative, they
// are considered to be counting back from the 
// end of the string.
// so passing in Start = 5 and End = -4 equates
// to the substring starting at index 5
// and ending at index Str.Len - 4
internal gs_string
StringSubstring(gs_string Str, s64 Start, s64 End)
{
  u64 StartIndex = (u64)Start;
  u64 EndIndex   = (u64)End;
  if (Start < 0) StartIndex = Str.Len + Start;
  if (End   < 0) EndIndex   = Str.Len + End;
  if (StartIndex >= EndIndex || StartIndex > Str.Len) return (gs_string){0};
  End = Min(End, Str.Len);
  
  gs_string Result = {};
  Result.Data = Str.Data + StartIndex;
  Result.Len  = EndIndex - StartIndex;
  Result.Cap = Result.Len;
  
  return Result;
}

//
// Split At (two out strings)
//

internal void
StringSplit(gs_string In, u64 Index, gs_string* Before, gs_string* After)
{
  u64 LenBefore = Index;
  u64 LenAfter  = In.Len - Index;
  if (Before->Cap >= LenBefore)
  {
    *Before = StringSubstring(In, 0, LenBefore);
  }
  if (After->Cap >= LenAfter)
  {
    *After = StringSubstring(In, LenBefore, In.Len);
  }
}

//
// Equals
//

internal bool
StringEqualsStringStartLen(gs_string A, gs_string B, u64 Start, u64 Len)
{
  if (Start >= A.Len || Start >= B.Len) return false;
  if (Start + Len > A.Len || Start + Len > B.Len) return false;
  for (u64 i = Start; i < Len; i++)
  {
    if (A.Data[i] != B.Data[i]) return false;
  }
  return true;
}

internal bool
StringEqualsString(gs_string A, gs_string B)
{
  if (A.Len != B.Len) return false;
  return StringEqualsStringStartLen(A, B, 0, A.Len);
}

//
// Reverse String
//

internal void
StringReverse(gs_string Str)
{
  s64 i = 0;
  s64 j = (s64)Str.Len - 1;
  while (i < j)
  {
    u8 T = Str.Data[i];
    Str.Data[i] = Str.Data[j];
    Str.Data[j] = T;
    i++;
    j--;
  }
}

//
// Copy String
//

internal void
StringCopy(gs_string Src, gs_string* Dest)
{
  u64 CopyLen = Min(Src.Len, Dest->Cap);
  for (u64 i = 0; i < CopyLen; i++)
  {
    Dest->Data[i] = Src.Data[i];
  }
  Dest->Len = CopyLen;
}

internal void
StringCopyCharArrayLen(char* Src, u64 Len, gs_string* Dest)
{
  u64 i = 0;
  for (; i < Min(Dest->Cap, Len) && Src[i] != 0; i++)
  {
    Dest->Data[i] = Src[i];
  }
  Dest->Len = i;
}

internal void
StringCopyCharArray(char* Src, gs_string* Dest)
{
  u64 Len = CStringLength(Src);
  StringCopyCharArrayLen(Src, Len, Dest);
}

#ifdef GS_MEMORY_H

internal gs_string
PushString(gs_memory_arena* A, u64 Cap)
{
  gs_string Result = {};
  Result.Cap = Cap;
  Result.Len = 0;
  Result.Data = PushArray(A, u8, Cap);
  return Result;
}

# ifdef GS_PRINTF_H

internal gs_string
PushStringF(gs_memory_arena* A, u64 Cap, char* Format, ...)
{
  va_list Args;
  va_start(Args, Format);
  gs_mprintfv_result R = MPrintFVVarArgs(A, Format, Args, false);
  va_end(Args);
  
  gs_string Result = {};
  Result.Cap = R.Length;
  Result.Len = R.Length;
  Result.Data = (u8*)R.Str;
  
  return Result;
}

# endif // GS_PRINTF_H
#endif // GS_MEMORY_H

#endif //GS_STRING_H
