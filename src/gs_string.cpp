////////////////////////////////////////////////////////////////
//        Init, Alloc, Realloc and Free
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

////////////////////////////////////////////////////////////////
//        Basic String Operations
////////////////////////////////////////////////////////////////

static bool
StringsEqual (string A, string B)
{
    return false;
}

static string
CopyString (string String)
{
    string Result = {};
    return Result;
}

static void
CopyStringTo (string Source, string* Dest)
{
    
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
    
}

static void
SafeInsertChar (string* String, char Char, int32_t Index)
{
    
}

static void
RemoveCharAt (string* String, int32_t Index)
{
    
}

static string
Substring (string* String, int32_t Start, int32_t End)
{
    string Result = {};
    return Result;
}



