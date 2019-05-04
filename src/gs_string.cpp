// Utility
static int32_t GSStringMin (int32_t A, int32_t B); 
static int32_t GSStringAbs (int32_t A); 
static float   GSStringAbsF (float A);
static float   GSStringPowF (float N, int32_t Power);

// Setup
static void   InitializeString (string* String, char* Data, int32_t DataSize);
static string InitializeString (char* Data, int32_t DataSize);
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
static uint32_t CharToUInt (char C);
static bool     CharArraysEqual (char* A, int32_t ALength, char* B, int32_t BLength);
static void     ReverseCharArray (char* Array, int32_t Length);

// String
static bool    StringsEqual (string A, string B);
static bool    StringEqualsCharArray (string String, char* CharArray);
static int32_t FindFirstChar (string String, char C);
static void    SetStringToChar (string* Dest, char C, int32_t Count);
static void    SetStringToCharArray (string* Dest, char* Source);
static void    ConcatString (string* Dest, string Source);
static void    ConcatString (string* Dest, string Source, int32_t Length);
static void    CopyStringTo (string Source, string* Dest);
static void    CopyCharArray (char* Source, char* Dest, int32_t DestLength);
static void    InsertChar (string* String, char Char, int32_t Index);
static void    RemoveCharAt (string* String, int32_t Index);
static string  Substring (string* String, int32_t Start, int32_t End);

// Parsing
static uint32_t ParseUnsignedInt (char* String, int32_t Length);
static int32_t  UIntToString (uint32_t Int, char* String, int32_t Length);
static int32_t  ParseSignedInt (char* String, int32_t Length);
static int32_t  IntToString (int32_t Int, char* String, int32_t Length);
static int32_t  IntToString (int32_t Int, char* String, int32_t Length, int32_t Offset);
static float    ParseFloat (char* String, int32_t Length);
static int32_t  FloatToString(float Float, char *String, int32_t Length, int32_t AfterPoint);

////////////////////////////////////////////////////////////////
//        String Utility Functions
////////////////////////////////////////////////////////////////

static int32_t
GSStringMin (int32_t A, int32_t B)
{
    return (A < B ? A : B);
}

static int32_t
GSStringAbs (int32_t A)
{
    return (A < 0 ? -A : A);
}

static float
GSStringAbsF (float A)
{
    return (A < 0 ? -A : A);
}

static float
GSStringPowF (float N, int32_t Power)
{
    float Result = N;
    for(int32_t i = 1; i < Power; i++) { Result *= N; }
    return Result;
}

////////////////////////////////////////////////////////////////
//        Init and Clear
////////////////////////////////////////////////////////////////

static void
InitializeString (string* String, char* Data, int32_t DataSize)
{
    String->Data = Data;
    String->Max = DataSize;
    String->Length = 0;
}

static string
InitializeString (char* Data, int32_t DataSize)
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

static uint32_t CharToUInt (char C) { 
    uint32_t Result = (C - '0');
    return Result; 
}

static bool
CharArraysEqual (char* A, int32_t ALength, char* B, int32_t BLength)
{
    bool Result = false;
    if (ALength == BLength)
    {
        Result = true;
        char* AIter = A;
        char* BIter = B;
        for (int32_t i = 0; i < ALength; i++)
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
ReverseCharArray (char* Array, int32_t Length)
{
    char* ForwardIter = Array;
    char* BackwardIter = Array + Length - 1;
    for (int32_t i = 0; i < (Length / 2); i++)
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
        for (int32_t i = 0; i < A.Length; i++)
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

static bool
StringEqualsCharArray (string String, char* CharArray)
{
    bool Result = true;
    
    char* S = String.Data;
    char* C = CharArray;
    
    int32_t Count = 0;
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

static int32_t
FindFirstChar (string String, char C)
{
    int32_t Result = -1;
    
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
SetStringToChar (string* Dest, char C, int32_t Count)
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
    for (int32_t i = 0; i < Source.Length; i++)
    {
        *Dst++ = *Src++;
        Dest->Length++;
    }
}

static void
ConcatString (string* Dest, string Source, int32_t Length)
{
    Assert(Length < Source.Length);
    Assert((Dest->Length + Length) <= Dest->Max);
    
    char* Dst = Dest->Data + Dest->Length;
    char* Src = Source.Data;
    for (int32_t i = 0; i < Length; i++)
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
    int32_t CopyLength = GSStringMin(Source.Length, Dest->Max);
    for (int i = 0; i < CopyLength; i++)
    {
        *Dst++ = *Src++;
    }
    Dest->Length = Source.Length;
}

static void
CopyCharArray (char* Source, char* Dest, int32_t DestLength)
{
    char* Src = Source;
    char* Dst = Dest;
    int32_t i = 0; 
    while (*Src && i < DestLength)
    {
        *Src++ = *Dst++;
        i++;
    }
}

static void
InsertChar (string* String, char Char, int32_t Index)
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
RemoveCharAt (string* String, int32_t Index)
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
Substring (string* String, int32_t Start, int32_t End)
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

static uint32_t
ParseUnsignedInt (char* String, int32_t Length)
{
    Assert(IsNumeric(*String));
    
    char* Iter = String;
    uint32_t Result = 0;
    for (int32_t i = 0; i < Length; i++)
    {
        Result = CharToUInt(*Iter++) + (Result * 10);
    }
    return Result;
}

static int32_t
UIntToString (uint32_t Int, char* String, int32_t Length)
{
    int32_t Remaining = Int;
    int32_t CharsCopied = 0;
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

static int32_t
ParseSignedInt (char* String, int32_t Length)
{
    Assert(Length > 0);
    
    int32_t Negative = 1;
    int32_t LengthRemaining = Length;
    int32_t Result = 0;
    char* Iter = String;
    
    if (*Iter == '-') { 
        LengthRemaining--;
        *Iter++; 
        Negative = -1; 
    }
    
    for (int32_t i = 0; i < LengthRemaining; i++)
    {
        Result = CharToUInt(*Iter++) + (Result * 10);
    }
    
    Result *= Negative;
    
    return Result;
}

static int32_t
IntToString (int32_t Int, char* String, int32_t Length)
{
    int32_t Remaining = Int;
    int32_t CharsCopied = 0;
    char* Iter = String;
    
    bool Negative = Remaining < 0;
    Remaining = GSStringAbs(Remaining);
    
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

static int32_t 
IntToString (int32_t Int, char* String, int32_t Length, int32_t Offset)
{
    char* StringStart = String + Offset;
    int32_t LengthWritten = IntToString(Int, StringStart, Length - Offset);
    return LengthWritten;
}

static float
ParseFloat (char* String, int32_t Length)
{
    int32_t Negative = 1;
    int32_t LengthRemaining = Length;
    float Result = 0;
    char* Iter = String;
    
    if (*Iter == '-') { 
        LengthRemaining--;
        *Iter++; 
        Negative = -1; 
    }
    
    for (int32_t i = 0; i < LengthRemaining; i++)
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
        int32_t PlacesAfterPoint = 0;
        
        for (int32_t i = 0; i < LengthRemaining; i++)
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
        
        AfterPoint = AfterPoint / GSStringPowF(10, PlacesAfterPoint);
        Result += AfterPoint;
    }
    
    Result *= Negative;
    
    return Result;
}

static int32_t
FloatToString(float Float, char *String, int32_t Length, int32_t AfterPoint)
{
    int32_t IPart = (int32_t)Float;
    float FPart = GSStringAbsF(Float - (float)IPart);
    
    int32_t i = IntToString(IPart, String, Length);
    
    if (AfterPoint > 1)
    {
        String[i++] = '.';
        
        int32_t FPartInt = FPart * GSStringPowF(10, AfterPoint);
        i += IntToString(FPartInt, String, Length, i);
    }
    
    return i;
}

////////////////////////////////////////////////////////////////
//        PrinfF
////////////////////////////////////////////////////////////////


