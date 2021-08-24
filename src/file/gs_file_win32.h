/* date = May 11th 2021 5:09 pm */

#ifndef GS_FILE_WIN32_H
#define GS_FILE_WIN32_H

typedef struct win32_file_handle
{
  HANDLE Handle;
  struct win32_file_handle* NextFree;
} win32_file_handle;

typedef struct win32_file_handler
{
  gs_memory_arena Arena;
  win32_file_handle* FileHandleFreeList;
} win32_file_handler;

internal win32_file_handle*
Win32FileHandleTake(u8* UserData)
{
  win32_file_handle* Result = 0;
  win32_file_handler* W = (win32_file_handler*)UserData;
  if (W->FileHandleFreeList)
  {
    Result = W->FileHandleFreeList;
    W->FileHandleFreeList = Result->NextFree;
  } else {
    Result = PushStruct(&W->Arena, win32_file_handle);
  }
  Result->NextFree = 0;
  return Result;
}

internal void
Win32FileHandleFree(u8* UserData, gs_file_handle* FileHandle)
{
  win32_file_handler* W = (win32_file_handler*)UserData;
  win32_file_handle* H = (win32_file_handle*)FileHandle->PlatformData;
  H->Handle = INVALID_HANDLE_VALUE;
  H->NextFree = W->FileHandleFreeList;
  W->FileHandleFreeList = H;
  FileHandle->PlatformData = 0;
}

internal bool
Win32FileHandleValidate(u8* UserData, gs_file_handle* FileHandle, HANDLE* HandleOut)
{
  bool Result = true;
  win32_file_handle* H = (win32_file_handle*)FileHandle->PlatformData;
  if (!FileHandleIsValid(*FileHandle) || H->Handle == INVALID_HANDLE_VALUE)
  {
    Win32FileHandleFree(UserData, FileHandle);
    FileHandle->Flags = 0;
    Result = false;
  } else {
    *HandleOut = H->Handle;
    Assert(*HandleOut != INVALID_HANDLE_VALUE);
  }
  return Result;
}

PLATFORM_FILE_OPEN(PFileOpenWin32)
{
  gs_file_handle Result = {0};
  
  DWORD CreationDisposition = 0;
  switch (CreateMode)
  {
    case FileCreate_AlwaysOverwrite: CreationDisposition = CREATE_ALWAYS; break;
    case FileCreate_NewOnly: CreationDisposition = CREATE_NEW; break;
    case FileCreate_OpenOrCreate: CreationDisposition = OPEN_ALWAYS; break;
    case FileCreate_OpenOnly: CreationDisposition = OPEN_EXISTING; break;
    case FileCreate_OpenAndClear: CreationDisposition = TRUNCATE_EXISTING; break;
    InvalidDefaultCase;
  }
  
  DWORD DesiredAccess = 0;
  if (AccessMode & FileAccess_Read) DesiredAccess |= GENERIC_READ;
  if (AccessMode & FileAccess_Write) DesiredAccess |= GENERIC_WRITE;
  if (AccessMode & FileAccess_Execute) DesiredAccess |= GENERIC_EXECUTE;
  
  GENERIC_READ;
  DWORD ShareMode = 0;
  LPSECURITY_ATTRIBUTES SecurityAttributes = 0;
  
  HANDLE FileHandle = CreateFileA(Path, 
                                  DesiredAccess, 
                                  ShareMode,
                                  SecurityAttributes,
                                  CreationDisposition,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL);
  if (FileHandle != INVALID_HANDLE_VALUE)
  {
    win32_file_handle* H = Win32FileHandleTake(UserData);
    H->Handle = FileHandle;
    
    Result.Flags = FileHandle_IsValid;
    Result.PlatformData = (u8*)H;
  }
  else
  {
    gs_file_error ErrResult = FileError_NoError;
    s32 Error = GetLastError();
    switch (Error)
    {
      case ERROR_ACCESS_DENIED:
      {
        ErrResult = FileError_AccessDenied;
      } break;
      
      // NOTE(PS): occurs if AccessMode is set to OpenAlways,
      // CreateAlways - the existing file is overwritten
      // and GetLastError will return this value
      case ERROR_ALREADY_EXISTS:
      {
        ErrResult = FileError_AlreadyExists;
      } break;
      
      // NOTE(PS): only occurs if AccessMode is set to 
      // CreateNew. The existing file is not overwritten
      // and GetLastError will return this value
      case ERROR_FILE_EXISTS:
      {
        ErrResult = FileError_AlreadyExists;
      } break;
      
      case ERROR_FILE_NOT_FOUND:
      {
        ErrResult = FileError_FileNotFound;
      } break;
      
      case ERROR_PIPE_BUSY:
      {
        ErrResult = FileError_PipeBusy;
      } break;
      
      // NOTE(PS): occurs in the case that the file requested
      // was requested elsewhere with sharing permissions that
      // conflict with the permissions this request needed
      case ERROR_SHARING_VIOLATION:
      {
        ErrResult = FileError_SharingViolation;
      } break;
      
      case ERROR_PATH_NOT_FOUND:
      {
        ErrResult = FileError_PathNotFound;
      } break;
      
      InvalidDefaultCase;
    }
    
    if (ErrorOut) *ErrorOut = ErrResult;
  }
  
  return Result;
}

PLATFORM_FILE_CLOSE(PFileCloseWin32)
{
  win32_file_handle* H = (win32_file_handle*)FileHandle->PlatformData;
  s32 Result = CloseHandle(H->Handle);
  s32 Error = GetLastError();
  if (!Result)
  {
    gs_file_error ErrResult = FileError_NoError;
    switch (Error)
    {
      case ERROR_INVALID_HANDLE:
      {
        ErrResult = FileError_InvalidHandle;
      } break;
      
      InvalidDefaultCase;
    }
    if (ErrorOut) *ErrorOut = ErrResult;
  }
  
  Win32FileHandleFree(UserData, FileHandle);
  return true;
}

PLATFORM_GET_FILE_INFO(GetFileInfoWin32)
{
  return false;
}

internal gs_file_error
Win32HandleReadWriteError ()
{
  gs_file_error Result = FileError_NoError;
  
  int Error = GetLastError();
  switch (Error) 
  {
    case ERROR_ACCESS_DENIED:
    {
      Result = FileError_AccessDenied;
    } break;
    
    case ERROR_IO_PENDING:
    {
      Result = FileError_IOPending;
    } break;
    
    case ERROR_OPERATION_ABORTED:
    {
      Result = FileError_OperationAborted;
    } break;
    
    // These are errors I'm not sure how I would want to 
    // handle. The InvalidDefaultCase is there to make sure
    // I know when I need to handle them explicitly
    case ERROR_NOT_ENOUGH_QUOTA:
    case ERROR_BROKEN_PIPE:
    {
      InvalidCodePath;
    } break;
    
    // These happen if too many requests are happening
    // simultaneously
    // TODO(PS): we want some way to let the
    // user know to try again in a moment. 
    case ERROR_INVALID_USER_BUFFER:
    case ERROR_NOT_ENOUGH_MEMORY:
    InvalidDefaultCase;
  }
  
  return Result;
}

PLATFORM_FILE_READ(PFileReadWin32)
{
  u64 Result = 0;
  HANDLE Win32Handle = INVALID_HANDLE_VALUE;
  if (Win32FileHandleValidate(UserData, FileHandle, &Win32Handle)) 
  {
    LPOVERLAPPED Overlapped = NULL;
    DWORD BytesRead = 0;
    if (ReadFile(Win32Handle, 
                 (LPVOID)Dest->Memory, 
                 Dest->Size, 
                 (LPDWORD)&BytesRead,
                 Overlapped))
    {
      Result = (u64)BytesRead;
    } else {
      gs_file_error ErrResult = Win32HandleReadWriteError();
      if (ErrorOut) *ErrorOut = ErrResult;
      Result = 0;
    }
  }
  return Result;
}

PLATFORM_FILE_WRITE(PFileWriteWin32)
{
  u64 Result = 0;
  HANDLE Win32Handle = INVALID_HANDLE_VALUE;
  if (Win32FileHandleValidate(UserData, FileHandle, &Win32Handle)) 
  {
    LPOVERLAPPED Overlapped = NULL;
    DWORD BytesWritten = 0;
    if (WriteFile(Win32Handle,
                  Data.Memory,
                  Data.Size,
                  &BytesWritten,
                  Overlapped))
    {
      Result = (u64)BytesWritten;
    } else {
      gs_file_error ErrResult = Win32HandleReadWriteError();
      if (ErrorOut) *ErrorOut = ErrResult;
      Result = 0;
    }
    
  }
  return Result;
}

PLATFORM_FILE_TELL(PFileTellWin32)
{
  s64 Result = 0;
  HANDLE Win32Handle = INVALID_HANDLE_VALUE;
  if (Win32FileHandleValidate(UserData, FileHandle, &Win32Handle))
  {
    SetLastError(NO_ERROR);
    DWORD MoveResult = SetFilePointer(Win32Handle,
                                      0,
                                      NULL,
                                      FILE_CURRENT);
    s32 Error = GetLastError();
    if (MoveResult != INVALID_SET_FILE_POINTER)
    {
      Result = (s64)MoveResult;
    } else {
      gs_file_error ErrResult = FileError_NoError;
      switch(Error)
      {
        case ERROR_INVALID_HANDLE:
        {
          ErrResult = FileError_InvalidHandle;
        } break;
        
        // NOTE(PS): Tried to move the pointer further back
        // than the beginning of the file
        // Windows doesn't move the pointer in this case
        // We have this fall through since we aren't asking 
        // the pointer to move - so NEGATIVE_SEEK is completely
        // invalid
        case ERROR_NEGATIVE_SEEK:
        
        // NOTE(PS): AFAICT this shouldn't encounter other errors
        InvalidDefaultCase;
      }
      
      if (ErrorOut) *ErrorOut = ErrResult;
    }
  }
  return Result;
}

PLATFORM_FILE_SEEK(PFileSeekWin32)
{
  s64 Result = 0;
  HANDLE Win32Handle = INVALID_HANDLE_VALUE;
  if (Win32FileHandleValidate(UserData, FileHandle, &Win32Handle))
  {
    DWORD MoveMethod = 0;
    switch (Base)
    {
      case FileLandmark_Start:   { MoveMethod = FILE_BEGIN; } break;
      case FileLandmark_Current: { MoveMethod = FILE_CURRENT; } break;
      case FileLandmark_End:     { MoveMethod = FILE_END; } break;
    }
    
    s32* DistToMove = (s32*)&Offset;
    
    SetLastError(NO_ERROR);
    int Error = GetLastError();
    DWORD MoveResult = SetFilePointer(Win32Handle,
                                      DistToMove[0],
                                      (PLONG)&DistToMove[1],
                                      MoveMethod);
    
    Error = GetLastError();
    if (MoveResult != INVALID_SET_FILE_POINTER && Error == NO_ERROR)
    {
      s32 DistMovedLow = (s32)MoveResult;
      s32 DistMovedHigh = (s32)DistToMove[1];
      Result = (DistMovedHigh << 4 | DistMovedLow);
    } else {
      gs_file_error ErrResult = FileError_NoError;
      switch(Error)
      {
        case ERROR_INVALID_HANDLE:
        {
          ErrResult = FileError_InvalidHandle;
        } break;
        
        // NOTE(PS): Tried to move the pointer further back
        // than the beginning of the file
        // Windows doesn't move the pointer in this case
        case ERROR_NEGATIVE_SEEK:
        {
          ErrResult = FileError_NegativeSeek;
          Result = 0;
        } break;
        
        InvalidDefaultCase;
      }
      
      if (ErrorOut) *ErrorOut = ErrResult;
    }
  }
  return Result;
}

PLATFORM_FILE_DELETE(PFileDeleteWin32)
{
  b8 Result = (b8)DeleteFileA(Path);
  s32 Error = GetLastError();
  if (!Result)
  {
    gs_file_error ErrResult = FileError_NoError;
    switch (Error)
    {
      case ERROR_FILE_NOT_FOUND:
      {
        ErrResult = FileError_FileNotFound;
      } break;
      
      case ERROR_ACCESS_DENIED:
      {
        ErrResult = FileError_AccessDenied;
      } break;
      
      InvalidDefaultCase;
    }
    if (ErrorOut) *ErrorOut = ErrResult;
  }
  return Result;
}

internal gs_file_handler 
CreatePlatformFileHandler(gs_allocator A)
{
  win32_file_handler* W = AllocStruct(A, win32_file_handler, "root");
  W->Arena = MemoryArenaCreate(4096, 8, A, 0, 0, (char*)"Win32 File Arena");
  
  gs_file_handler Result = FileHandlerCreate(A,
                                             GetFileInfoWin32,
                                             PFileOpenWin32,
                                             PFileCloseWin32,
                                             PFileReadWin32,
                                             PFileWriteWin32,
                                             PFileTellWin32,
                                             PFileSeekWin32,
                                             PFileDeleteWin32,
                                             (u8*)W,
                                             (u64)MAX_PATH);
  return Result;
}

internal void 
CleanupPlatformFileHandler(gs_file_handler* H, gs_allocator A)
{
  win32_file_handler* W = (win32_file_handler*)H->UserData;
  MemoryArenaFree(&W->Arena);
  FreeStruct(A, W, win32_file_handler);
  FileHandlerCleanup(H, A);
}

#endif //GS_FILE_WIN32_H
