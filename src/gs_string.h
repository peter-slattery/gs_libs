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

#define StrExpand(s) (u32)(s).Len, (char*)(s).Data

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

//
// Append String
//

internal u64
StringAppend(gs_string* Dest, u8* A, u64 ALen)
{
  u64 i = 0;
  for (; i < Min(Dest->Cap - Dest->Len, ALen); i++)
  {
    Dest->Data[Dest->Len + i] = A[i];
  }
  Dest->Len += i;
  return i;
}
internal u64
StringAppendString(gs_string* Dest, gs_string A)
{
  return StringAppend(Dest, A.Data, A.Len);
}
internal u64
StringAppendCString(gs_string* Dest, char* A)
{
  u64 Len = CStringLength(A);
  return StringAppend(Dest, (u8*)A, Len);
}

//
// Encodings
//

// Bytes To Binary, Octal, Decimal, Hex
gs_string DigitsBinary   = LitStr("01");
gs_string DigitsOctal    = LitStr("01234567");
gs_string DigitsDecimal  = LitStr("0123456789");
gs_string DigitsHex      = LitStr("0123456789ABCDEF");
gs_string DigitsHexLower = LitStr("0123456789abcdef");

internal char
GetCharFromByte(u8 Byte, gs_string Digits)
{
  Assert(Byte < Digits.Len);
  char Result = Digits.Data[Byte];
  return Result;
}

internal void
ToStringU32(u32 Int, gs_string Digits, gs_string* Dest)
{
  u64 Start = Dest->Len;
  u32 Remaining = Int;
  while (Dest->Len < Dest->Cap && Remaining > 0)
  {
    u8 Byte = (u8)(Remaining % Digits.Len);
    Dest->Data[Dest->Len++] = GetCharFromByte(Byte, Digits);
    Remaining = Remaining / Digits.Len;
  }
  
  gs_string Range = {};
  Range.Data = Dest->Data + Start;
  Range.Len  = Dest->Len - Start;
  StringReverse(Range);
}
internal void
ToStringU64(u64 Int, gs_string Digits, gs_string* Dest)
{
  u64 Start = Dest->Len;
  u64 Remaining = Int;
  while (Dest->Len < Dest->Cap && Remaining > 0)
  {
    u8 Byte = (u8)(Remaining % Digits.Len);
    Dest->Data[Dest->Len++] = GetCharFromByte(Byte, Digits);
    Remaining = Remaining / Digits.Len;
  }
  
  gs_string Range = {};
  Range.Data = Dest->Data + Start;
  Range.Len  = Dest->Len - Start;
  StringReverse(Range);
}


// Base-64
// Source: http://web.mit.edu/freebsd/head/contrib/wpa/src/utils/base64.c

global char EncodingTable_Base64[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

global u8 DecodingTable_Base64[256] = {
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,62,0x80,0x80,0x80,63,52,53,
  54,55,56,57,58,59,60,61,0x80,0x80,
  0x80,0,0x80,0x80,0x80,0,1,2,3,4,
  5,6,7,8,9,10,11,12,13,14,
  15,16,17,18,19,20,21,22,23,24,
  25,0x80,0x80,0x80,0x80,0x80,0x80,26,27,28,
  29,30,31,32,33,34,35,36,37,38,
  39,40,41,42,43,44,45,46,47,48,
  49,50,51,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
  0x80,0x80,0x80,0x80,0x80,0x80,
};

internal u64
Encode_Base64OutputLen(u64 Len)
{
  u64 OutputLen = Len * 4 / 3 + 4; // 3-byte blocks to 4-byte
  OutputLen += OutputLen / 72; // line feeds
  OutputLen++; // null termination
  if (OutputLen < Len) return 0; // int overflow
  return OutputLen;
}

internal u64
Encode_Base64(u8* Data, u64 Len, u8* Output, u64 OutputCap)
{
  Assert(Output);
  Assert(OutputCap >= Encode_Base64OutputLen(Len));
  
  u8* End = Data + Len;
  u8* In = Data;
  u8* Pos = Output;
  u64 LineLen = 0;
  while (End - In >= 3)
  {
    *Pos++ = EncodingTable_Base64[In[0] >> 2];
    *Pos++ = EncodingTable_Base64[((In[0] & 0x03) << 4) | (In[1] >> 4)];
    *Pos++ = EncodingTable_Base64[((In[1] & 0x0f) << 2) | (In[2] >> 6)];
    *Pos++ = EncodingTable_Base64[In[2] & 0x3f];
    
    In += 3;
    LineLen += 4;
    if (LineLen >= 72) {
      *Pos++ = '\n';
      LineLen = 0;
    }
  }
  
  if (End - In)
  {
    *Pos++ = EncodingTable_Base64[In[0] >> 2];
    if (End - In == 1) {
      *Pos++ = EncodingTable_Base64[(In[0] & 0x03) << 4];
    } else {
      *Pos++ = EncodingTable_Base64[((In[0] & 0x03) << 4) | (In[1] >> 4)];
      *Pos++ = EncodingTable_Base64[((In[1] & 0x0f) << 2) | (In[2] >> 6)];
    }
    *Pos++ = '=';
    LineLen += 4;
  }
  
  //if (LineLen) *Pos++ = '\n';
  *Pos = '\0';
  
  return (Pos - Output);
}

internal u64
Decode_Base64OutputLen(u8* Data, u64 Len)
{
  u64 Result = 0;
  u64 Count = 0;
  for (u64 i = 0; i < Len; i++)
  {
    if (DecodingTable_Base64[Data[i]] != 0x80) Count += 1;
  }
  Result = Count / 4 * 3;
  return Result;
}

internal u64
Decode_Base64(u8* Data, u64 Len, u8* Output, u64 OutputCap)
{
  Assert(Output);
  Assert(Decode_Base64OutputLen(Data, Len) >= OutputCap);
  
  u8* Pos = Output;
  u8 Block[4];
  u64 Count = 0;
  u64 Pad = 0;
  u8 Tmp = 0;
  for (u64 i = 0; i < Len; i++) 
  {
    Tmp = DecodingTable_Base64[Data[i]];
    if (Tmp == 0x80) continue;
    
    if (Data[i] == '=') Pad++;
    
    Block[Count++] = Tmp;
    if (Count == 4)
    {
      *Pos++ = (Block[0] << 2) | (Block[1] >> 4);
      *Pos++ = (Block[1] << 4) | (Block[2] >> 2);
      *Pos++ = (Block[2] << 6) | (Block[3]);
      Count = 0;
      if (Pad) 
      {
        if      (Pad == 1) { Pos -= 1; }
        else if (Pad == 2) { Pos -= 2; }
        else { 
          // invalid padding
          return 0;
        }
        break;
      }
    }
  }
  return (Pos - Output);
}

#ifdef GS_MEMORY_H

internal u8*
Encode_Base64Alloc(u8* Data, u64 Len, u64* OutputLen, gs_memory_arena* Arena)
{
  u64 AllocLen = Encode_Base64OutputLen(Len);
  u8* Result = PushSize(Arena, AllocLen).Memory;
  *OutputLen = Encode_Base64(Data, Len, Result, AllocLen);
  return Result;
}
internal gs_string
EncodeStr_Base64Alloc(gs_string Str, gs_memory_arena* Arena)
{
  gs_string Result = {};
  Result.Data = Encode_Base64Alloc(Str.Data, Str.Len, &Result.Len, Arena);
  return Result;
}

internal u8*
Decode_Base64Alloc(u8* Data, u64 Len, u64* OutputLen, gs_memory_arena* Arena)
{
  u64 AllocLen = Decode_Base64OutputLen(Data, Len);
  u8* Result = PushSize(Arena, AllocLen).Memory;
  *OutputLen = Decode_Base64(Data, Len, Result, AllocLen);
  return Result;
}
internal gs_string
DecodeStr_Base64Alloc(gs_string Str, gs_memory_arena* Arena)
{
  gs_string Result = {};
  Result.Data = Decode_Base64Alloc(Str.Data, Str.Len, &Result.Len, Arena);
  return Result;
}

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
