
#ifdef DEBUG
#ifndef Assert
#define Assert(condition) if (!(condition)) { *((int*)0) = 5; }
#endif
#endif

#ifdef USE_STD_INT
#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#elif !defined(GS_TYPES)

#define GSINT64(s) (s) ## L
#define GSUINT64(s) (s) ## UL

typedef unsigned char          u8;
typedef unsigned short int     u16;
typedef unsigned int           u32;
typedef unsigned long long int u64;

typedef signed char   s8;
typedef short int     s16;
typedef int           s32;
typedef long long int s64;

#define INT8_MIN   (-128)
#define INT16_MIN  (-32767-1)
#define INT32_MIN  (-2147483647-1)
#define INT64_MIN  (-GSINT64(9223372036854775807)-1)

#define INT8_MAX   (127)
#define INT16_MAX  (32767)
#define INT32_MAX  (2147483647)
#define INT64_MAX  (GSINT64(9223372036854775807))

#define UINT8_MAX  (255)
#define UINT16_MAX (65535)
#define UINT32_MAX (4294967295U)
#define UINT64_MAX (GSUINT64(18446744073709551615))

#define GS_TYPES
#endif


////////////////////////////////////////////////////////////////
//        String 
////////////////////////////////////////////////////////////////

struct string
{
    char* Data;
    s32 Length;
    s32 Max;
};


////////////////////////////////////////////////////////////////
//        String Memory
////////////////////////////////////////////////////////////////

struct slot_header
{
    slot_header* Next;
    s32 Size;
};

struct slot_arena
{
    u8* Memory;
    s32 SlotSize;
    s32 SlotCount;
    slot_header* FreeList;
};

struct contiguous_slot_count_result
{
    s32 Count;
    slot_header* LastContiguousSlot;
};


////////////////////////////////////////////////////////////////
//        String Function Declarations
////////////////////////////////////////////////////////////////

// Utility
#if !defined GS_LANGUAGE_H

static void    GSZeroMemory (u8* Memory, s32 Size);
static s32     GSMin (s32 A, s32 B); 
static s32     GSAbs (s32 A); 
static float   GSAbsF (float A);
static float   GSPowF (float N, s32 Power);

#endif

// Setup
static void   InitializeString (string* String, char* Data, s32 DataSize);
static string InitializeString (char* Data, s32 DataSize);
static void   ClearString (string* String);

// Char/Char Array
static bool     IsSlash (char C);
static bool     IsNewline (char C);
static bool     IsWhitespace (char C);
static bool     IsAlpha (char C);
static bool     IsUpper (char C);
static bool     IsLower (char C);
static bool     IsNumeric (char C);
static bool     ToUpper (char C);
static bool     ToLower (char C);
static bool     IsAlphaNumeric (char C);
static u32 CharToUInt (char C);
static bool     CharArraysEqual (char* A, s32 ALength, char* B, s32 BLength);
static void     ReverseCharArray (char* Array, s32 Length);

// String
static bool    StringsEqual (string A, string B);
static bool    StringEqualsCharArray (string String, char* CharArray);
static s32 FindFirstChar (string String, char C);
static void    SetStringToChar (string* Dest, char C, s32 Count);
static void    SetStringToCharArray (string* Dest, char* Source);
static void    ConcatString (string* Dest, string Source);
static void    ConcatString (string* Dest, string Source, s32 Length);
static void    CopyStringTo (string Source, string* Dest);
static void    CopyCharArray (char* Source, char* Dest, s32 DestLength);
static void    InsertChar (string* String, char Char, s32 Index);
static void    RemoveCharAt (string* String, s32 Index);
static string  Substring (string* String, s32 Start, s32 End);

// Parsing
static u32 ParseUnsignedInt (char* String, s32 Length);
static s32  UIntToString (u32 Int, char* String, s32 Length);
static s32  ParseSignedInt (char* String, s32 Length);
static s32  IntToString (s32 Int, char* String, s32 Length);
static s32  IntToString (s32 Int, char* String, s32 Length, s32 Offset);
static float    ParseFloat (char* String, s32 Length);
static s32  FloatToString(float Float, char *String, s32 Length, s32 AfterPoint);


////////////////////////////////////////////////////////////////
//        String Memory Function Declarations
////////////////////////////////////////////////////////////////

static s32                          CalculateSlotCountFromSize (s32 RequestedSize, s32 SlotSize);
static bool                         SlotsAreContiguous (slot_header* First, slot_header* Second);
static contiguous_slot_count_result CountContiguousSlots (slot_header* First);
static slot_header*                 GetSlotAtOffset(slot_header* First, s32 Offset);
static slot_header*                 InsertSlotIntoList (slot_header* NewSlot, slot_header* ListStart);
static void                         AllocStringFromStringArena (string* String, s32 Size, slot_arena* Storage);
static string                       AllocStringFromStringArena (s32 Size, slot_arena* Storage);
static void                         FreeToStringArena (string* String, slot_arena* Storage);
static void                         ReallocFromStringArena (string* String, s32 NewSize, slot_arena* Storage);

////////////////////////////////////////////////////////////////
//        String Utility Functions
////////////////////////////////////////////////////////////////

#if !defined GS_LANGUAGE_H

static void
GSZeroMemory (u8* Memory, s32 Size)
{
    for (int i = 0; i < Size; i++) { Memory[i] = 0; }
}

static s32
GSMin (s32 A, s32 B)
{
    return (A < B ? A : B);
}

static s32
GSAbs (s32 A)
{
    return (A < 0 ? -A : A);
}

static float
GSAbs (float A)
{
    return (A < 0 ? -A : A);
}

static float
GSPow (float N, s32 Power)
{
    float Result = N;
    for(s32 i = 1; i < Power; i++) { Result *= N; }
    return Result;
}

#endif

////////////////////////////////////////////////////////////////
//        Init and Clear
////////////////////////////////////////////////////////////////

static void
InitializeString (string* String, char* Data, s32 DataSize)
{
    String->Data = Data;
    String->Max = DataSize;
    String->Length = 0;
}

static string
InitializeString (char* Data, s32 DataSize)
{
    string Result = {};
    Result.Data = Data;
    Result.Max = DataSize;
    Result.Length = 0;
    return Result;
}

static void
ClearString (string* String)
{
    String->Data = 0;
    String->Max = 0;
    String->Length = 0;
}

////////////////////////////////////////////////////////////////
//        Basic Char Operations
////////////////////////////////////////////////////////////////

static bool IsSlash (char C) { return ((C == '\\') || (C == '/')); }
static bool IsNewline (char C) { return (C == '\n') || (C == '\r'); }
static bool IsWhitespace (char C) { return (C == ' ') || (C == '\t') || IsNewline(C); }
static bool IsAlpha (char C)
{
    // TODO(Peter): support UTF8 chars
    return ((C >= 'A') && (C <= 'Z')) || ((C >= 'a') && (C <= 'z'));
}
static bool IsUpper (char C)
{
    return ((C >= 'A') && (C <= 'Z'));
}
static bool IsLower (char C)
{
    return ((C >= 'a') && (C <= 'z'));
}
static bool IsNumeric (char C)
{
    return (C >= '0') && (C <= '9');
}
static bool IsAlphaNumeric (char C)
{
    return IsAlpha(C) || IsNumeric(C);
}

static u32 CharToUInt (char C) { 
    u32 Result = (C - '0');
    return Result; 
}

static s32
CharArrayLength (char* Array)
{
    char* C = Array;
    s32 Result = 0;
    while (*C)
    {
        *C++;
        Result++;
    }
    return Result;
}

static bool
CharArraysEqual (char* A, s32 ALength, char* B, s32 BLength)
{
    bool Result = false;
    if (ALength == BLength)
    {
        Result = true;
        char* AIter = A;
        char* BIter = B;
        for (s32 i = 0; i < ALength; i++)
        {
            if(*AIter++ != *BIter++)
            {
                Result = false;
                break;
            }
        }
    }
    return Result;
}

static void
ReverseCharArray (char* Array, s32 Length)
{
    char* ForwardIter = Array;
    char* BackwardIter = Array + Length - 1;
    for (s32 i = 0; i < (Length / 2); i++)
    {
        char F = *ForwardIter;
        char B = *BackwardIter;
        *ForwardIter++ = B;
        *BackwardIter-- = F;
    }
}

////////////////////////////////////////////////////////////////
//        Basic String Operations
////////////////////////////////////////////////////////////////

static bool
StringsEqual (string A, string B)
{
    bool Result = false;
    
    if (A.Length == B.Length)
    {
        Result = true;
        char* AIter = A.Data;
        char* BIter = B.Data;
        for (s32 i = 0; i < A.Length; i++)
        {
            if (*AIter++ != *BIter++)
            {
                Result = false;
                break;
            }
        }
    }
    
    return Result;
}

#define MakeStringLiteral(array) MakeString((array), sizeof(array))
static string
MakeString (char* Array, s32 Length)
{
    string Result = {};
    Result.Data = Array;
    Result.Length = Length;
    Result.Max = Length;
    return Result;
}

static string
MakeString (char* Array)
{
    s32 Length = CharArrayLength (Array);
    return MakeString(Array, Length);
}

static bool
StringEqualsCharArray (string String, char* CharArray)
{
    bool Result = true;
    
    char* S = String.Data;
    char* C = CharArray;
    
    s32 Count = 0;
    while (*C && Count < String.Length)
    {
        if (*C++ != *S++)
        {
            Result = false;
            break;
        }
        Count++;
    }
    
    return Result;
}

static s32
FindFirstChar (string String, char C)
{
    s32 Result = -1;
    
    char* Iter = String.Data;
    for (int i = 0; i < String.Length; i++)
    {
        if (*Iter++ == C)
        {
            Result = i;
            break;
        }
    }
    
    return Result;
}

static void
SetStringToChar (string* Dest, char C, s32 Count)
{
    Assert(Count <= Dest->Max);
    
    char* Iter = Dest->Data;
    for (int i = 0; i < Count; i++)
    {
        *Iter++ = C;
    }
    Dest->Length = Count;
}

static void
SetStringToCharArray (string* Dest, char* Source)
{
    Dest->Length = 0;
    
    char* Src = Source;
    char* Dst = Dest->Data;
    while (*Src && Dest->Length < Dest->Max)
    {
        *Dst++ = *Src++;
        Dest->Length++;
    }
}

static void
ConcatString (string* Dest, string Source)
{
    Assert((Dest->Length + Source.Length) <= Dest->Max);
    
    char* Dst = Dest->Data + Dest->Length;
    char* Src = Source.Data;
    for (s32 i = 0; i < Source.Length; i++)
    {
        *Dst++ = *Src++;
        Dest->Length++;
    }
}

static void
ConcatString (string* Dest, string Source, s32 Length)
{
    Assert(Length < Source.Length);
    Assert((Dest->Length + Length) <= Dest->Max);
    
    char* Dst = Dest->Data + Dest->Length;
    char* Src = Source.Data;
    for (s32 i = 0; i < Length; i++)
    {
        *Dst++ = *Src++;
        Dest->Length++;
    }
}

static void
CopyStringTo (string Source, string* Dest)
{
    char* Src = Source.Data;
    char* Dst = Dest->Data;
    s32 CopyLength = GSMin(Source.Length, Dest->Max);
    for (int i = 0; i < CopyLength; i++)
    {
        *Dst++ = *Src++;
    }
    Dest->Length = Source.Length;
}

static void
CopyCharArray (char* Source, char* Dest, s32 DestLength)
{
    char* Src = Source;
    char* Dst = Dest;
    s32 i = 0; 
    while (*Src && i < DestLength)
    {
        *Src++ = *Dst++;
        i++;
    }
}

static void
InsertChar (string* String, char Char, s32 Index)
{
    Assert(Index >= 0 && Index < String->Max);
    Assert(String->Length < String->Max);
    
    char* Src = String->Data + String->Length;
    char* Dst = Src + 1;
    for (int i = String->Length - 1; i >= Index; i--)
    {
        *Dst-- = *Src--;
    }
    
    *(String->Data + Index) = Char;
    String->Length++;
}

static void
RemoveCharAt (string* String, s32 Index)
{
    Assert(Index >= 0 && Index < String->Max);
    
    char* Dst = String->Data + Index;
    char* Src = Dst + 1;
    for (int i = Index; i < String->Length; i++)
    {
        *Dst++ = *Src++;
    }
    *Dst = 0;
    String->Length--;
}

static string
Substring (string* String, s32 Start, s32 End)
{
    Assert(Start >= 0 && End > Start && End < String->Length);
    
    string Result = {};
    Result.Data = String->Data + Start;
    Result.Length = End - Start;
    return Result;
}

////////////////////////////////////////////////////////////////
//        String Parsing
////////////////////////////////////////////////////////////////

static u32
ParseUnsignedInt (char* String, s32 Length)
{
    Assert(IsNumeric(*String));
    
    char* Iter = String;
    u32 Result = 0;
    for (s32 i = 0; i < Length; i++)
    {
        Result = CharToUInt(*Iter++) + (Result * 10);
    }
    return Result;
}

static s32
UIntToString (u32 Int, char* String, s32 Length)
{
    s32 Remaining = Int;
    s32 CharsCopied = 0;
    char* Iter = String;
    while (Remaining > 0 && CharsCopied < Length)
    {
        *Iter++ = '0' + (Remaining % 10);
        Remaining /= 10;
        CharsCopied++;
    }
    ReverseCharArray(String, CharsCopied);
    return CharsCopied;
}

static s32
ParseSignedInt (char* String, s32 Length)
{
    Assert(Length > 0);
    
    s32 Negative = 1;
    s32 LengthRemaining = Length;
    s32 Result = 0;
    char* Iter = String;
    
    if (*Iter == '-') { 
        LengthRemaining--;
        *Iter++; 
        Negative = -1; 
    }
    
    for (s32 i = 0; i < LengthRemaining; i++)
    {
        Result = CharToUInt(*Iter++) + (Result * 10);
    }
    
    Result *= Negative;
    
    return Result;
}

static s32
IntToString (s32 Int, char* String, s32 Length)
{
    s32 Remaining = Int;
    s32 CharsCopied = 0;
    char* Iter = String;
    
    bool Negative = Remaining < 0;
    Remaining = GSAbs(Remaining);
    
    while (Remaining > 0 && CharsCopied < Length)
    {
        *Iter++ = '0' + (Remaining % 10);
        Remaining /= 10;
        CharsCopied++;
    }
    
    if (Negative)
    {
        *Iter++ = '-';
        CharsCopied++;
    }
    
    ReverseCharArray(String, CharsCopied);
    return CharsCopied;
}

static s32 
IntToString (s32 Int, char* String, s32 Length, s32 Offset)
{
    char* StringStart = String + Offset;
    s32 LengthWritten = IntToString(Int, StringStart, Length - Offset);
    return LengthWritten;
}

static float
ParseFloat (char* String, s32 Length)
{
    s32 Negative = 1;
    s32 LengthRemaining = Length;
    float Result = 0;
    char* Iter = String;
    
    if (*Iter == '-') { 
        LengthRemaining--;
        *Iter++; 
        Negative = -1; 
    }
    
    for (s32 i = 0; i < LengthRemaining; i++)
    {
        if (IsNumeric(*Iter))
        {
            Result = (float)CharToUInt(*Iter++) + (Result * 10);
        }
        else if (*Iter == '.' || *Iter == 0) 
        { 
            LengthRemaining -= i;
            break; 
        }
    }
    
    if (*Iter == '.')
    {
        *Iter++;
        float AfterPoint = 0;
        s32 PlacesAfterPoint = 0;
        
        for (s32 i = 0; i < LengthRemaining; i++)
        {
            if (IsNumeric(*Iter))
            {
                AfterPoint = (float)CharToUInt(*Iter++) + (AfterPoint * 10);
                PlacesAfterPoint++;
            }
            else
            {
                break;
            }
        }
        
        AfterPoint = AfterPoint / GSPow(10, PlacesAfterPoint);
        Result += AfterPoint;
    }
    
    Result *= Negative;
    
    return Result;
}

static s32
FloatToString(float Float, char *String, s32 Length, s32 AfterPoint)
{
    s32 IPart = (s32)Float;
    float FPart = GSAbs(Float - (float)IPart);
    
    s32 i = IntToString(IPart, String, Length);
    
    if (AfterPoint > 1)
    {
        String[i++] = '.';
        
        s32 FPartInt = FPart * GSPow(10, AfterPoint);
        i += IntToString(FPartInt, String, Length, i);
    }
    
    return i;
}

////////////////////////////////////////////////////////////////
//        PrintF
////////////////////////////////////////////////////////////////


static void
PrintF (string* Dest, char* Format, s32 FormatLength,  ...)
{
    
}


////////////////////////////////////////////////////////////////
//        String Memory Function Definitions
////////////////////////////////////////////////////////////////

static s32
CalculateSlotCountFromSize (s32 RequestedSize, s32 SlotSize)
{
    s32 SlotCount = RequestedSize / SlotSize;
    if (SlotCount * SlotSize < RequestedSize)
    {
        SlotCount += 1;
    }
    return SlotCount;
}

static bool
SlotsAreContiguous (slot_header* First, slot_header* Second)
{
    bool Result = false;
    u8* FirstSlotNextAddress = (u8*)First + First->Size;
    u8* SecondAddress = (u8*)Second;
    Result = FirstSlotNextAddress == SecondAddress;
    return Result;
}

static contiguous_slot_count_result
CountContiguousSlots (slot_header* First)
{
    Assert(First != 0);
    
    contiguous_slot_count_result Result = {};
    Result.Count = 1;
    
    slot_header* IterPrev = First;
    slot_header* Iter = First->Next;
    while (Iter && SlotsAreContiguous(IterPrev, Iter))
    {
        Result.Count++;
        IterPrev = Iter;
        Iter = Iter->Next;
    }
    
    Result.LastContiguousSlot = IterPrev;
    return Result;
}

static slot_header*
GetSlotAtOffset(slot_header* First, s32 Offset)
{
    slot_header* Iter = First;
    s32 Count = 0;
    while (Count < Offset && Iter)
    {
        Iter = Iter->Next;
        Count++;
    }
    return Iter;
}

static slot_header*
InsertSlotIntoList (slot_header* NewSlot, slot_header* ListStart)
{
    slot_header* List = ListStart;
    if (NewSlot < List)
    {
        NewSlot->Next = List;
        List = NewSlot;
    }
    else
    {
        slot_header* PrevIter = List;
        slot_header* Iter = List->Next;
        while (Iter && NewSlot > Iter)
        {
            PrevIter = Iter;
            Iter = Iter->Next;
        }
        
        Assert(PrevIter);
        if (PrevIter) 
        { 
            PrevIter->Next = NewSlot;
        }
        
        if (Iter)
        {
            NewSlot->Next = Iter;
        }
    }
    return List;
}

static void
AllocStringFromStringArena (string* String, s32 Size, slot_arena* Storage)
{
    s32 SlotCount = CalculateSlotCountFromSize(Size, Storage->SlotSize);
    slot_header* Slot = Storage->FreeList;
    slot_header* PrevSlot = 0;
    while (Slot)
    {
        contiguous_slot_count_result ContiguousSlots = CountContiguousSlots(Slot);
        if (ContiguousSlots.Count >= SlotCount)
        {
            slot_header* NextStartSlot = GetSlotAtOffset(Slot, SlotCount);
            if (PrevSlot)
            {
                PrevSlot->Next = NextStartSlot;
            }
            else
            {
                Storage->FreeList = NextStartSlot;
            }
            break;
        }
        else
        {
            PrevSlot = Slot;
            Slot = Slot->Next; 
        }
    }
    
    if (Slot)
    {
        String->Data = (char*)Slot;
        GSZeroMemory((u8*)String->Data, SlotCount * Storage->SlotSize);
        String->Max = SlotCount * Storage->SlotSize;
        String->Length = 0;
    }
}

static string
AllocStringFromStringArena (s32 Size, slot_arena* Storage)
{
    string Result = {0};
    AllocStringFromStringArena(&Result, Size, Storage);
    return Result;
}

static void
FreeToStringArena (string* String, slot_arena* Storage)
{
    u8* Base = (u8*)(String->Data);
    u8* End = Base + String->Max - 1;
    u8* MemoryEnd = Storage->Memory + (Storage->SlotSize * Storage->SlotCount);
    Assert((Base >= Storage->Memory) && (End < MemoryEnd));
    Assert((String->Max % Storage->SlotSize) == 0);
    
    s32 SizeReclaimed = 0;
    slot_header* Slot = (slot_header*)Base;
    while (SizeReclaimed < String->Max)
    {
        Slot->Size = Storage->SlotSize;
        Storage->FreeList = InsertSlotIntoList(Slot, Storage->FreeList);
        SizeReclaimed += Storage->SlotSize;
        Slot = (slot_header*)(Base + SizeReclaimed);
    }
    
    String->Data = 0;
    String->Length = 0;
    String->Max = 0;
}

static void
ReallocFromStringArena (string* String, s32 NewSize, slot_arena* Storage)
{
    string NewString = AllocStringFromStringArena(NewSize, Storage);
    CopyStringTo(*String, &NewString);
    FreeToStringArena(String, Storage);
    *String = NewString;
}