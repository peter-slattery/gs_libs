#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "../src/gs_tests.h"

#define GS_PLATFORM_IMPLEMENTATION
#include "../src/gs_platform.h"
#include "../src/gs_types.h"
#define ASSERTS_ACTIVE 1
#include "../src/gs_assert.h"
#include "../src/memory/gs_memory.h"
#include "../src/file/gs_file.h"

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  
  gs_allocator A = CreatePlatformAllocator();
  
  TestGroup("File Handler - File Handles")
  {
    gs_file_handler F = CreatePlatformFileHandler(A);
    
    gs_file_error Error = FileError_NoError;
    gs_file_handle Handle = {0};
    // Handle is invalid when ZII
    GlobalTest(!FileHandleIsValid(Handle));
    // Handle is still invalid when open file didn't work
    Handle = FileOpen(F, "non-existent-file.foo", FileAccess_Read, FileCreate_OpenOnly, 0, &Error);
    GlobalTest(!FileHandleIsValid(Handle));
    GlobalTest(Error == FileError_FileNotFound);
    
    // Handle is valid once a file has been opened
    Handle = FileOpen(F, "./test_dir/test_file.txt", FileAccess_Read, FileCreate_OpenOnly, 0, &Error);
    GlobalTest(FileHandleIsValid(Handle));
    
    // Handle is invalid after closing
    FileClose(F, &Handle, &Error);
    GlobalTest(!FileHandleIsValid(Handle));
    
    // Error when closing after file has been closed
    FileClose(F, &Handle, &Error);
    GlobalTest(Error == FileError_InvalidHandle);
    
    CleanupPlatformFileHandler(&F, A);
  }
  
  TestGroup("File Handler - Create/Delete")
  {
    gs_file_handler F = CreatePlatformFileHandler(A);
    
    char* FileName = "create-delete-file.bar";
    gs_file_error Error = FileError_NoError;
    gs_file_handle Handle = {0};
    
    // Opening a non-existent file results in an invalid handle
    Handle = FileOpen(F, FileName, FileAccess_Read, FileCreate_OpenOnly, 0, &Error);
    GlobalTest(!FileHandleIsValid(Handle));
    GlobalTest(Error == FileError_FileNotFound);
    
    // Opening a file with OpenOrCreate creates the file
    Handle = FileOpen(F, FileName, FileAccess_Read, FileCreate_OpenOrCreate, 0, &Error);
    GlobalTest(FileHandleIsValid(Handle));
    
    FileClose(F, &Handle, 0);
    
    b8 Deleted = FileDelete(F, FileName, 0, &Error);
    GlobalTest(Deleted);
    
    Deleted = FileDelete(F, FileName, 0, &Error);
    GlobalTest(!Deleted);
    GlobalTest(Error == FileError_FileNotFound);
    
    CleanupPlatformFileHandler(&F, A);
  }
  
  TestGroup("File Handler - Read")
  {
    gs_file_handler F = CreatePlatformFileHandler(A);
    gs_file_error Error = FileError_NoError;
    
    // Handle is valid once a file has been opened
    gs_file_handle Handle = FileOpen(F, "./test_dir/test_file.txt", FileAccess_Read, FileCreate_OpenOnly, 0, &Error);
    
    // Read from the file
    gs_data Data = AllocData(A, 4096, "root");
    u64 SizeRead = FileRead(F, &Handle, 4096, &Data, &Error);
    
    // Test that the file contents are what we expect
    bool Success = true;
    char* TestStr = "hello, sailor!";
    char* SrcStr = (char*)Data.Memory;
    char* TestAt = TestStr;
    char* SrcAt = SrcStr;
    for (;*SrcAt != 0 && TestAt != 0; SrcAt++, TestAt++)
    {
      if (*SrcAt != *TestAt)
      {
        Success = false;
      }
    }
    if (*SrcAt != 0) Success = false;
    GlobalTest(Success);
    
    FileClose(F, &Handle, &Error);
    
    // TODO(PS): make sure there isn't a memory leak here?
    
    CleanupPlatformFileHandler(&F, A);
  }
  
  TestGroup("File Handler - Write")
  {
    gs_file_handler F = CreatePlatformFileHandler(A);
    gs_file_error Error = FileError_NoError;
    
    gs_file_handle Handle = FileOpen(F, "./test_dir/dump_file.dbin", FileAccess_Read | FileAccess_Write, FileCreate_OpenOrCreate, 0, &Error);
    
    s64 Pos = FileTell(F, &Handle, 0);
    GlobalTest(Pos == 0);
    
    gs_data Data = AllocData(A, 4096, "root");
    for (u64 i = 0; i < 4096; i++)
    {
      u8 Byte = (u8)(i % 256);
      Data.Memory[i] = Byte;
    }
    
    u64 SizeWritten = FileWrite(F, &Handle, Data, &Error);
    GlobalTest(SizeWritten == 4096);
    
    Pos = FileTell(F, &Handle, &Error);
    GlobalTest(Pos == SizeWritten);
    
    Pos = FileSeek(F, &Handle, FileLandmark_Start, 0, &Error);
    GlobalTest(Pos == 0);
    
    // Clear Data before we fill it so we know its not just
    // filled with the previous data
    for (u64 i = 0; i < 4096; i++)
    {
      Data.Memory[i] = 0;
    }
    
    // Read what we wrote back into Data and make sure we're reading
    // the proper size back
    u64 SizeRead = FileRead(F, &Handle, SizeWritten, &Data, &Error);
    GlobalTest(SizeRead == SizeWritten);
    
    bool Success = true;
    for (u64 i = 0; i < 4096; i++)
    {
      u8 Byte = (u8)(i % 256);
      Success &= Data.Memory[i] == Byte;
    }
    GlobalTest(Success);
    
    // Moving the file pointer to a negative position results in an error
    // 1. move to position 100
    Pos = FileSeek(F, &Handle, FileLandmark_Start, 100, &Error);
    // 2. attempt to move to position -100
    Pos = FileSeek(F, &Handle, FileLandmark_Start, -200, &Error);
    GlobalTest(Pos == 0);
    GlobalTest(Error == FileError_NegativeSeek);
    
    FileClose(F, &Handle, 0);
    GlobalTest(!FileHandleIsValid(Handle));
    
    CleanupPlatformFileHandler(&F, A);
  }
  
  return 0;
}