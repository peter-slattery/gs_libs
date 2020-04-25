//
// File: gs_file.h
// Author: Peter Slattery
// Creation Date: 2020-03-30
//
//
// Usage:
// To use, include this file in ONE file in your program, and define one of values
// listed below, based on which implementation you'd like to use
//
// Defines
// GS_FILE_CSTDLIB - includes the C standard libary implementation.
//     Depends on: <stdio.h>
//     Uses fopen, fseek, fread, and fclose
// GS_FILE_WIN32
//     Depends on: <windows.h>
// GS_FILE_OSX
//     Depends on: <sys/stat.h>
//
#ifndef GS_FILE_H

#ifndef GS_TYPES_H
# error "gs_file.h relies on gs_types.h. Please include gs_types.h before gs_file.h"
#endif

#ifdef GS_FILE_CSTDLIB

static u64
gs_GetFileSize(gs_file_handler FileHandler, gs_const_string Path)
{
    Assert(IsNullTerminated(Path));
    u64 FileSize = 0;
    FILE* ReadFile = fopen(NullTermPath.Str, "r");
    if (ReadFile)
    {
        fseek(ReadFile, 0, SEEK_END);
        // NOTE(Peter): Leaving space for a null terminated string
        FileSize = (u64)ftell(ReadFile) + 1;
        fclose(ReadFile);
    }
    return FileSize;
}

static gs_file
gs_ReadEntireFileAndNullTerminate (gs_file_handler FileHandler, gs_const_string Path, gs_data Memory)
{
    Assert(Memory.Memory != 0);
    Assert(Memory.Length > 0);
    Assert(IsNullTerminated(Path));
    gs_file Result = {0};
    Result.Path = Path;
    FILE* ReadFile = fopen(Path.Str, "r");
    if (ReadFile)
    {
        u64 ReadSize = (u64)fread(Memory.Memory, 1, Memory.Length - 1, ReadFile);
        Memory.Memory[ReadSize] = 0;
        fclose(ReadFile);
        if((ReadSize + 1) == FileLength)
        {
            Result.Memory = Memory;
        }
    }
    return Result;
}

static bool
gs_WriteEntireFile(char* Path, unsigned char* FileMemory, unsigned int FileLength)
{
    
}

#endif

#if defined(GS_FILE_WIN32) || defined(GS_PLATFORM_WIN32)

static u64
gs_GetFileSize(gs_file_handler FileHandler, gs_const_string Path)
{
    Assert(IsNullTerminated(Path));
    u64 FileSize = 0;
    HANDLE FileHandle = CreateFileA(Path.Str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        FileSize = (u64)GetFileSize(FileHandle, NULL) + 1;
        CloseHandle(FileHandle);
    }
    return FileSize;
}

static gs_file
gs_ReadEntireFileAndNullTerminate (gs_file_handler FileHandler, gs_const_string Path, gs_data Memory)
{
    Assert(DataIsNonEmpty(Memory));
    Assert(IsNullTerminated(Path));
    
    gs_file Result = {0};
    Result.Path = Path;
    
    HANDLE FileHandle = CreateFileA(Path.Str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD BytesRead = 0;
        if (ReadFile(FileHandle, (LPVOID)Memory.Memory, Memory.Size - 1, (LPDWORD)(&BytesRead), NULL))
        {
            Memory.Memory[Memory.Size - 1] = 0;
            Result.Data = Memory;
        }
        else
        {
            u32 Error = GetLastError();
            // :ErrorReporting
            // TODO(Peter): Error Handling - now that we're passing a file handler in here,
            // and are thinking about passing in a thread context as well, this might be a good
            // way to get error stuff out ie. have a generic ReportError(ThreadContext, "FormatString", ...)
            // that can be configured, or could default to printf var args or whatever
            Assert(0);
        }
        CloseHandle(FileHandle);
    }
    
    return Result;
}

static bool
gs_WriteEntireFile(gs_file_handler FileHandler, gs_const_string Path, gs_data Memory)
{
    Assert(DataIsNonEmpty(Memory));
    Assert(IsNullTerminated(Path));
    
    bool Success = false;
    HANDLE FileHandle = CreateFileA(Path.Str, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle  != INVALID_HANDLE_VALUE)
    {
        DWORD BytesWritten = 0;
        if (WriteFile(FileHandle, Memory.Memory, Memory.Size, &BytesWritten, NULL))
        {
            Success = (BytesWritten == Memory.Size);
        }
        else
        {
            // :ErrorReporting
            u32 Error = GetLastError();
            Assert(0);
        }
    }
    
    return Success;
}

#endif

#if defined(GS_FILE_OSX) || defined(GS_PLATFORM_OSX)

static u64
gs_GetFileSize(gs_file_handler FileHandler, gs_const_string Path)
{
    Assert(IsNullTerminated(Path));
    u64 Filesize = 0;
    int FileHandle = open(Path.Str, O_RDONLY);
    if (FileHandle)
    {
        stat FileStat = {0};
        fstat(FileHandle, &FileStat);
        
        Result = (u64)FileStat.st_size + 1;
        close(FileHandle);
    }
	return Result;
}

static gs_file
gs_ReadEntireFileAndNullTerminate (gs_file_handler FileHandler, gs_const_string Path, gs_data Memory)
{
    Assert(DataIsNonEmpty(Memory));
	Assert(IsNullTerminated(Path));
	int FileHandle = open(Path.Str, O_RDONLY);
    if (FileHandle)
    {
        stat FileStat = {0};
        fstat(FileHandle, &FileStat);
        if (FileStat.st_size <= FileSize)
        {
            read(FileHandle, Memory.Memory, Memory.Size - 1);
            Memory.Memory[Memory.Size - 1] = 0;
            Result = true;
        }
        else
        {
            // :ErrorReporting
        }
        close(FileHandle);
    }
    
	return Result;
}

static bool
gs_WriteEntireFile(gs_file_handler FileHandler, gs_const_string Path, gs_data Memory)
{
	Assert(DataIsNonEmpty(Memory));
	Assert(IsNullTerminated(Path));
    bool Result = false;
	
    int FileHandle = open(Path.Str, O_WRONLY | O_CREAT, 0777);
	if (FileHandle)
    {
        ssize_t SizeWritten = write(FileHandle, Memory.Memory, Memory.Size);
        Result = ((bool)SizeWritten == Memory.Size);
        if (!Result)
        {
            // :ErrorReporting
        }
        close(FileHandle);
    }
	return Result;
}

#endif

// NOTE(Peter): Not platform specific since all procedures have same names and same signatures
static gs_file_handler
gs_CreatePlatformFileHandler(gs_allocator* Allocator)
{
    gs_file_handler Result = CreateFileHandler(gs_GetFileSize,
                                               gs_ReadEntireFileAndNullTerminate,
                                               gs_WriteEntireFile,
                                               0, // Enumerate Directory
                                               Allocator);
    return Result;
}

#define GS_FILE_H
#endif // GS_FILE_H