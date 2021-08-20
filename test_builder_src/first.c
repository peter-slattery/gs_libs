/*
Takes in a single parameter, a directory where tests are stored
Recursively iterates through the directory finding all .test.cpp files
and builds a .bat file that compiles each of them, and then runs them
*/

#if defined(__clang__)
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"

#if defined(_WIN32) || defined(_WIN64)
# undef PLATFORM_WINDOWS
# define PLATFORM_WINDOWS 1
#endif

#if defined(__APPLE__) && defined(__MACH__)
# undef PLATFORM_OSX
# define PLATFORM_OSX 1
#endif

#if defined(PLATFORM_WIN32)
# include <windows.h>
# include "platform_win32.h"
#elif defined(PLATFORM_OSX)
# include <dirent.h>
# include <errno.h>
# include "platform_osx.h"
#endif

#include "settings.h"

typedef struct str_list{
    char* Str;
    struct str_list* Next;
} str_list;

str_list* SearchDirs = 0;
str_list* Win32_BuildStr = 0;
str_list* OSX_BuildStr = 0;

bool StrIsDir (char* Path, int Len)
{
    char LastChar = Path[Len - 1];
    bool Result = (LastChar == '\\' || LastChar == '/');
    return Result;
}

str_list* AllocStr (int Length)
{
    str_list* Next = (str_list*)malloc(sizeof(str_list) + (sizeof(char) * Length));
    Next->Str = (char*)(Next + 1);
    Next->Next = 0;
    return Next;
}

str_list* PushStrOnSearchList (int Length)
{
    str_list* Next = AllocStr(Length);
    if (SearchDirs != 0) 
    {
        Next->Next = SearchDirs;
    }
    SearchDirs = Next;
    return Next;
}

str_list* PushStrOnBuildString_OSX ()
{
    str_list* Next = AllocStr(2048);
    if (OSX_BuildStr != 0) 
    {
        Next->Next = OSX_BuildStr;
    }
    OSX_BuildStr = Next;
    return Next;
}

str_list* PushStrOnBuildString_Win32 ()
{
    str_list* Next = AllocStr(2048);
    if (Win32_BuildStr != 0) 
    {
        Next->Next = Win32_BuildStr;
    }
    Win32_BuildStr = Next;
    return Next;
}

char* CatDirs (str_list* Dest, char* ParentDir, int ParentDirLength, char* Dir, int DirLength)
{
    // print the path in the form <parent_dir><dir>
    // we know at this point that parent_dir ends in a /
    char* PrintAt = Dest->Str;
    if (ParentDirLength > 0) 
    {
        if (ParentDir[ParentDirLength - 1] == '*') 
        {
            ParentDirLength -= 1;
        }
        PrintAt += sprintf(PrintAt, "%.*s", ParentDirLength, ParentDir);
    }
    PrintAt += sprintf(PrintAt, "%.*s", DirLength, Dir);
    return PrintAt;
}

char* AllocCatDirs (char* ParentDir, char* Dir)
{
    int DirLength = strlen(Dir);
    int ParentDirLength = strlen(ParentDir);
    int Length = ParentDirLength + DirLength + 3;
    str_list* Next = PushStrOnSearchList(Length);
    CatDirs(Next, ParentDir, ParentDirLength, Dir, DirLength);
    return Next->Str;
}

char* AllocSearchString (char* Dir, char* ParentDir)
{
    char* Result = 0;
    int DirLength = strlen(Dir);
    
    int ParentDirLength = 0;
    if (ParentDir)
    {
        ParentDirLength = strlen(ParentDir);
        if (!StrIsDir(ParentDir, ParentDirLength))
        {
            printf("Supplied parent path must be a directory (end in \\ or /)\n");
            return Result;
        }
    }
    
    int Length = ParentDirLength + DirLength + 3;
    str_list* Next = PushStrOnSearchList(Length);
    
    char* PrintAt = CatDirs(Next, ParentDir, ParentDirLength, Dir, DirLength);
    if (Dir[DirLength - 1] != '\\' && Dir[DirLength - 1] != '/')
    {
        PrintAt += sprintf(PrintAt, "\\");
    }

    #if defined(PLATFORM_WIN32)
    PrintAt += sprintf(PrintAt, "*");
    #endif

    Result = Next->Str;
    return Result;;
}

void FreeAllSearchDirs () {
    str_list* At = SearchDirs;
    while (At != 0)
    {
        str_list* NextAt = At->Next;
        free(At);
        At = NextAt;
    }
}

void FreeAllBuildStrs_ (str_list* At)
{
    while (At != 0)
    {
        str_list* NextAt = At->Next;
        free(At);
        At = NextAt;
    }
}
void FreeAllBuildStrs () {
    FreeAllBuildStrs_(Win32_BuildStr);
    FreeAllBuildStrs_(OSX_BuildStr);
}

void PushBuildString (char* TestFilePath, char* FileName)
{
    int SecondToLastPeriodIndex = -1;
    int LastPeriodIndex = -1;
    for (char* C = FileName; *C != 0; C++) {
        if (*C == '.')
        {
            SecondToLastPeriodIndex = LastPeriodIndex;
            LastPeriodIndex = C - FileName;
        }
    }
    
    char* TestStr = FileName + SecondToLastPeriodIndex + 1;
    if (SecondToLastPeriodIndex < 0 || strncmp(TestStr, "test", 4) != 0)
    {
        // NOTE(PS): we only want to build and test files that end in .test.<ext>
        return;
    }
    
    str_list* Win32_BuildString = PushStrOnBuildString_Win32();
    char* At = Win32_BuildString->Str;
    At += sprintf(At, "echo COMPILING %.*s...\n\n", LastPeriodIndex, FileName);
    At += sprintf(At, "set TestFileNoExtensions=%.*s\n", LastPeriodIndex, FileName);
    At += sprintf(At, "set TestFileFullPath=%s\n", TestFilePath);
    for (int i = 0; i < BuildCommandsLength_Win32; i++)
    {
        At += sprintf(At, "%s", BuildCommands_Win32[i]);
    }
    At += sprintf(At, "echo.\n\n");

    str_list* OSX_BuildString = PushStrOnBuildString_OSX();
    At = OSX_BuildString->Str;
    At += sprintf(At, "echo COMPILING %.*s...\n\n", LastPeriodIndex, FileName);
    At += sprintf(At, "TestFileNoExtensions=%.*s\n", LastPeriodIndex, FileName);
    At += sprintf(At, "TestFileFullPath=%s\n", TestFilePath);
    for (int i = 0; i < BuildCommandsLength_OSX; i++)
    {
        At += sprintf(At, "%s", BuildCommands_OSX[i]);
    }
    At += sprintf(At, "echo.\n\n");
    
}

void BuildTestBatchFile (char* SearchDir, char* ParentDir)
{
    char* QueryString = AllocSearchString(SearchDir, ParentDir);
    
    file_search_query Query = Platform_BeginFileSearch(QueryString);
    if (Query.HasFile)
    {
        do {
            if (Query.IsDirectory)
            {
                char* SubDirName = Query.FileName;
                if (strcmp(SubDirName, ".") != 0 && 
                    strcmp(SubDirName, "..") != 0)
                {
                    BuildTestBatchFile(SubDirName, SearchDir);
                }
            }
            else
            {
                char* FullFilePath = AllocCatDirs(QueryString, Query.FileName);
                PushBuildString(FullFilePath, Query.FileName);
            }
        } while (Platform_FindNextFile(&Query));
    }
}

void
WriteOutputFile(
    FILE* OutputFileHandle,
    str_list* BuildStr,
    char** SetupCommands, int SetupCommandsLength,
    char** BuildCommands, int BuildCommandLength,
    char** CleanupCommands, int CleanupCommandsLength)
{
    if (OutputFileHandle)
    {
        // Write Preamble
        char* Str = (char*)malloc(2048 * sizeof(char));
        char* At = Str;
        for (int i = 0; i < SetupCommandsLength; i++)
        {
            At += sprintf(At, "%s", SetupCommands[i]);
        }
        
        int StrLen = strlen(Str);
        fwrite(Str, StrLen, 1, OutputFileHandle);
        
        // Write Tests
        for (str_list* At = BuildStr; At != 0; At = At->Next)
        {
            int AtLength = strlen(At->Str) * sizeof(char);
            int WrittenLength = fwrite(At->Str, AtLength, 1, OutputFileHandle);
        }
        
        // Write Cleanup
        At = Str;
        for (int i = 0; i < CleanupCommandsLength; i++)
        {
            At += sprintf(At, "%s", CleanupCommands[i]);
        }
        
        StrLen = strlen(Str);
        fwrite(Str, StrLen, 1, OutputFileHandle);
        
        fclose(OutputFileHandle);
    } else {
        printf("Error: Could not create build_and_run_tests.bat\nAborting\n");
    }
}

int main(int ArgCount, char** Args)
{
    bool DisplayHelp = ((ArgCount == 1) ||
                        (ArgCount == 2 && !strcmp(Args[0], "--help")));
    if (DisplayHelp)
    {
        printf("Usage:\n");
        printf("\ttest_builder.exe --help: Print this message\n");
        printf("\ttest_builder.exe <path_to_test_dir>: will iterate over all *.test.cpp files at that path and build a .bat file which compiles and runs each of them\n");
        return(0);
    }
    
    // Look up the first file at TestDir
    char* TestDir = Args[1];
    BuildTestBatchFile(TestDir, 0);
    
    FILE* OutputFileHandle_Win32 = fopen("build_and_run_tests.bat", "w");
    WriteOutputFile(
        OutputFileHandle_Win32, 
        Win32_BuildStr,
        SetupCommands_Win32, SetupCommandsLength_Win32,
        BuildCommands_Win32, BuildCommandsLength_Win32,
        CleanupCommands_Win32, CleanupCommandsLength_Win32);

    FILE* OutputFileHandle_OSX = fopen("build_and_run_tests.sh", "w");
    WriteOutputFile(
        OutputFileHandle_OSX,
        OSX_BuildStr, 
        SetupCommands_OSX, SetupCommandsLength_OSX,
        BuildCommands_OSX, BuildCommandsLength_OSX,
        CleanupCommands_OSX, CleanupCommandsLength_OSX);
    
    // Clean Up
    FreeAllSearchDirs();
    FreeAllBuildStrs();
    printf("\n\nFinished\n");
    
    return(0);
}