/* date = May 11th 2021 3:57 pm */

#ifndef GS_FILE_H
#define GS_FILE_H

#if !defined(GS_FILE_PROFILE_FUNC)
# define GS_FILE_PROFILE_FUNC
#endif

typedef enum
{
  FileError_NoError,
  
  FileError_AccessDenied,
  FileError_AlreadyExists,
  FileError_PipeBusy,
  FileError_SharingViolation,
  
  FileError_InvalidHandle,
  
  FileError_IOPending,
  FileError_OperationAborted,
  
  FileError_FileNotFound,
  FileError_PathNotFound,
  
  FileError_NegativeSeek,
  
  FileError_NoDataRead,
  
  FileError_Count,
} gs_file_error;

global char* FileErrorStrings[] = {
  (char*)"FileError_NoError",
  
  (char*)"FileError_AccessDenied",
  (char*)"FileError_AlreadyExists",
  (char*)"FileError_PipeBusy",
  (char*)"FileError_SharingViolation",
  
  (char*)"FileError_InvalidHandle",
  
  (char*)"FileError_IOPending",
  (char*)"FileError_OperationAborted",
  
  (char*)"FileError_FileNotFound",
  
  (char*)"FileError_NegativeSeek",
  
  (char*)"FileError_NoDataRead",
  
  (char*)"FileError_Count",
};

enum gs_file_handle_flag
{
  FileHandle_IsValid = 0x1,
};

enum gs_file_flag
{
  FileFlag_Exists = 0x1,
  FileFlag_IsDirectory = 0x2,
  
};

enum gs_file_access_mode
{
  FileAccess_Read = 0x1,
  FileAccess_Write = 0x2,
  FileAccess_Execute = 0x4,
};

enum gs_file_create_mode
{
  FileCreate_AlwaysOverwrite,
  FileCreate_NewOnly, // fails if the file exists
  FileCreate_OpenOrCreate, 
  FileCreate_OpenOnly,
  FileCreate_OpenAndClear, // fails if the file exists
};

enum gs_file_landmark
{
  FileLandmark_Start,
  FileLandmark_Current,
  FileLandmark_End,
};

typedef struct gs_file_info
{
  char* Path;
  char* AbsolutePath;
  u64 FileSize;
  u64 CreationTime;
  u64 LastWriteTime;
  b8 Flags;
} gs_file_info;

typedef struct gs_file_handle 
{
  b8 Flags;
  u8* PlatformData;
} gs_file_handle;

#define PLATFORM_GET_FILE_INFO(name) bool name(char* Path, gs_file_info* Dest, u8* UserData)
#define PLATFORM_FILE_OPEN(name) \
gs_file_handle name(char* Path, enum gs_file_access_mode AccessMode, enum gs_file_create_mode CreateMode, u8 Flags, gs_file_error* ErrorOut, u8* UserData)
#define PLATFORM_FILE_CLOSE(name) b8 name(gs_file_handle* FileHandle, gs_file_error* ErrorOut, u8* UserData)
#define PLATFORM_FILE_READ(name) u64 name(gs_file_handle* FileHandle, u64 ReadSize, gs_data* Dest, gs_file_error* ErrorOut, u8* UserData)
#define PLATFORM_FILE_WRITE(name) u64 name(gs_file_handle* FileHandle, gs_data Data, gs_file_error* ErrorOut, u8* UserData)
#define PLATFORM_FILE_TELL(name) s64 name(gs_file_handle* FileHandle, gs_file_error* ErrorOut, u8* UserData)
#define PLATFORM_FILE_SEEK(name) b8 name(gs_file_handle* FileHandle, enum gs_file_landmark Base, s64 Offset, gs_file_error* ErrorOut, u8* UserData)
#define PLATFORM_FILE_DELETE(name) b8 name(char* Path, u8 Flags, gs_file_error* ErrorOut, u8* UserData)
//#define PLATFORM_LIST_DIRECTORY(name) 

typedef PLATFORM_GET_FILE_INFO(platform_get_file_info);
typedef PLATFORM_FILE_OPEN(platform_file_open);
typedef PLATFORM_FILE_CLOSE(platform_file_close);
typedef PLATFORM_FILE_READ(platform_file_read);
typedef PLATFORM_FILE_WRITE(platform_file_write);
typedef PLATFORM_FILE_TELL(platform_file_tell);
typedef PLATFORM_FILE_SEEK(platform_file_seek);
typedef PLATFORM_FILE_DELETE(platform_file_delete);
//typedef PLATFORM_LIST_DIRECTORY(platform_list_directory);

typedef struct gs_file_handler
{
  platform_get_file_info* PGetFileInfo;
  platform_file_open*   PFileOpen;
  platform_file_close*  PFileClose;
  platform_file_read*   PFileRead;
  platform_file_write*  PFileWrite;
  platform_file_tell*   PFileTell;
  platform_file_seek*   PFileSeek;
  platform_file_delete* PFileDelete;
  // platform_list_directory* PListDirectory;
  
  gs_memory_arena* Arena;
  
  u8* UserData;
  
  u64 MaxPath;
} gs_file_handler;

PLATFORM_GET_FILE_INFO(PGetFileInfoNoOp)
{
  return false;
}
PLATFORM_FILE_OPEN(PFileOpenNoOp)
{
  return (gs_file_handle){0};
}
PLATFORM_FILE_CLOSE(PFileCloseNoOp)
{
  return false;
}
PLATFORM_FILE_READ(PFileReadNoOp)
{
  return false;
}
PLATFORM_FILE_WRITE(PFileWriteNoOp)
{
  return 0;
}
PLATFORM_FILE_TELL(PFileTellNoOp)
{
  return -1;
}
PLATFORM_FILE_SEEK(PFileSeekNoOp)
{
  return false;
}
PLATFORM_FILE_DELETE(PFileDeleteNoOp)
{
  return false;
}

// NOTE(PS): This just makes sure name exists, and if not, assigns
// the NoOp version of name instead.
// ie. if name = PFileSeek, then this evaluates to 
//     (PFileSeek) ? (PFileSeek) : (PFileSeekNoOp)
#define FILE_HANDLER_SAFE_PROC(name) (name) ? (name) : (name ## NoOp)

// NOTE(PS): cast function and struct pointers to proper data types
// for convenience
#define FileHandlerCreate(a,g,o,c,r,w,t,s,d,u,mp)         \
FileHandlerCreate_((a),                          \
(platform_get_file_info*)(g), \
(platform_file_open*)(o),     \
(platform_file_close*)(c),    \
(platform_file_read*)(r),     \
(platform_file_write*)(w),    \
(platform_file_tell*)(t),     \
(platform_file_seek*)(s),     \
(platform_file_delete*)(d),   \
(u8*)(u), \
(mp))
internal gs_file_handler
FileHandlerCreate_(gs_allocator Allocator,
                   platform_get_file_info* PGetFileInfo,
                   platform_file_open*     PFileOpen,
                   platform_file_close*    PFileClose,
                   platform_file_read*     PFileRead,
                   platform_file_write*    PFileWrite,
                   platform_file_tell*     PFileTell,
                   platform_file_seek*     PFileSeek,
                   platform_file_delete*   PFileDelete,
                   u8* UserData,
                   u64 MaxPath)
{
  gs_memory_arena* Arena = AllocStruct(Allocator, gs_memory_arena, "root");
  *Arena = MemoryArenaCreate(4096, 8, Allocator, 0, 0, (char*)"File Handler Arena");
  
  gs_file_handler Result = {};
  Result.PGetFileInfo = FILE_HANDLER_SAFE_PROC(PGetFileInfo);
  Result.PFileOpen    = FILE_HANDLER_SAFE_PROC(PFileOpen);
  Result.PFileClose   = FILE_HANDLER_SAFE_PROC(PFileClose);
  Result.PFileRead    = FILE_HANDLER_SAFE_PROC(PFileRead);
  Result.PFileWrite   = FILE_HANDLER_SAFE_PROC(PFileWrite);
  Result.PFileTell    = FILE_HANDLER_SAFE_PROC(PFileTell);
  Result.PFileSeek    = FILE_HANDLER_SAFE_PROC(PFileSeek);
  Result.PFileDelete  = FILE_HANDLER_SAFE_PROC(PFileDelete);
  Result.UserData     = UserData;
  Result.Arena        = Arena;
  Result.MaxPath      = MaxPath;
  
  return Result;
}

internal void
FileHandlerCleanup(gs_file_handler* H, gs_allocator A)
{
  MemoryArenaFree(H->Arena);
  FreeStruct(A, H->Arena, gs_memory_arena);
}

internal bool
FileHandleIsValid(gs_file_handle H)
{
  GS_FILE_PROFILE_FUNC;
  return H.Flags & FileHandle_IsValid;
}

internal void
FillOutFileInfo(gs_file_handler F, char* Path, gs_file_info* Dest)
{
  GS_FILE_PROFILE_FUNC;
  F.PGetFileInfo(Path, Dest, F.UserData);
}

internal gs_file_info
GetFileInfo(gs_file_handler F, char* Path)
{
  GS_FILE_PROFILE_FUNC;
  gs_file_info Result = {
    .Path = PushArray(F.Arena, char, F.MaxPath),
    .AbsolutePath = PushArray(F.Arena, char, F.MaxPath),
  };
  FillOutFileInfo(F, Path, &Result);
  return Result;
}

internal gs_file_handle
FileOpen(gs_file_handler F, char* Path, enum gs_file_access_mode AccessMode, enum gs_file_create_mode CreateMode, u8 Flags, gs_file_error* ErrorOut)
{
  GS_FILE_PROFILE_FUNC;
  gs_file_handle Result = F.PFileOpen(Path, AccessMode, CreateMode, Flags, ErrorOut, F.UserData);
  return Result;
}

internal b8
FileClose(gs_file_handler F, gs_file_handle* Handle, gs_file_error* ErrorOut)
{
  GS_FILE_PROFILE_FUNC;
  b8 Result = false;
  if (FileHandleIsValid(*Handle))
  {
    Result = F.PFileClose(Handle, ErrorOut, F.UserData);
    if (Result) {
      Handle->Flags = 0;
    }
  }
  else if (ErrorOut)
  {
    *ErrorOut = FileError_InvalidHandle;
  }
  return Result;
}

internal u64
FileRead(gs_file_handler F, gs_file_handle* Handle, u64 ReadSize, gs_data* Dest, gs_file_error* ErrorOut)
{
  GS_FILE_PROFILE_FUNC;
  u64 Result = 0;
  if (FileHandleIsValid(*Handle))
  {
    Result = F.PFileRead(Handle, ReadSize, Dest, ErrorOut, F.UserData);
  }
  else if (ErrorOut)
  {
    *ErrorOut = FileError_InvalidHandle;
  }
  return Result;
}

internal u64
FileWrite(gs_file_handler F, gs_file_handle* Handle, gs_data Data, gs_file_error* ErrorOut)
{
  GS_FILE_PROFILE_FUNC;
  u64 Result = 0;
  if (FileHandleIsValid(*Handle))
  {
    Result = F.PFileWrite(Handle, Data, ErrorOut, F.UserData);
  }
  else if (ErrorOut)
  {
    *ErrorOut = FileError_InvalidHandle;
  }
  return Result;
}

internal s64
FileTell(gs_file_handler F, gs_file_handle* Handle, gs_file_error* ErrorOut)
{
  GS_FILE_PROFILE_FUNC;
  s64 Result = 0;
  if (FileHandleIsValid(*Handle))
  {
    Result = F.PFileTell(Handle, ErrorOut, F.UserData);
  }
  else if (ErrorOut)
  {
    *ErrorOut = FileError_InvalidHandle;
  }
  return Result;
}

internal bool
FileSeek(gs_file_handler F, gs_file_handle* Handle, enum gs_file_landmark Base, u64 Offset, gs_file_error* ErrorOut)
{
  GS_FILE_PROFILE_FUNC;
  bool Result = false;
  if (FileHandleIsValid(*Handle))
  {
    Result = F.PFileSeek(Handle, Base, Offset, ErrorOut, F.UserData);
  }
  else if (ErrorOut)
  {
    *ErrorOut = FileError_InvalidHandle;
  }
  return Result;
}

internal u64
FileGetSize(gs_file_handler F, gs_file_handle* Handle, gs_file_error* ErrorOut)
{
  if (!FileHandleIsValid(*Handle)) {
    *ErrorOut = FileError_InvalidHandle;
    return 0;
  }
  
  if (FileSeek(F, Handle, FileLandmark_End, 0, ErrorOut))
  {
    s64 Size = FileTell(F, Handle, ErrorOut);
    FileSeek(F, Handle, FileLandmark_Start, 0, ErrorOut);
    if (*ErrorOut == FileError_NoError)
    {
      return Size;
    }
  }
  return 0;
}

internal bool
FileDelete(gs_file_handler F, char* Path, u8 Flags, gs_file_error* ErrorOut)
{
  GS_FILE_PROFILE_FUNC;
  bool Result = F.PFileDelete(Path, Flags, ErrorOut, F.UserData);
  return Result;
}

internal gs_data
FileReadAllAndClose(gs_file_handler F, char* Path, gs_memory_arena* Arena, gs_file_error* ErrorOut)
{
  GS_FILE_PROFILE_FUNC;
  gs_data Result = {0};
  
  *ErrorOut = FileError_NoError;
  gs_file_handle Handle = FileOpen(F, Path, FileAccess_Read, FileCreate_OpenOnly, 0, ErrorOut);
  if (FileHandleIsValid(Handle))
  {
    if (FileSeek(F, &Handle, FileLandmark_End, 0, ErrorOut))
    {
      s64 Size = FileTell(F, &Handle, ErrorOut);
      if (Size > 0)
      {
        if (!Arena) {
          Arena = F.Arena;
        } 
        Result = PushSize(Arena, Size + 1);
        FileSeek(F, &Handle, FileLandmark_Start, 0, ErrorOut);
        
        u64 ReadAmount = FileRead(F, &Handle, Size, &Result, ErrorOut);
        if (ReadAmount == (u64)Size)
        {
          Result.Memory[ReadAmount] = 0;
          
          // NOTE(PS): PushSize might have to align things
          // forward, making Size bigger than the actual
          // file size. This makes some file operations 
          // annoying, so we just reset it here to the 
          // file's actual size.
          Result.Size = ReadAmount;
        }
        else if (ErrorOut)
        {
          *ErrorOut = FileError_NoDataRead;
        }
      }
    }
    FileClose(F, &Handle, ErrorOut);
  }
  
  return Result;
}

#ifdef GS_PLATFORM_IMPLEMENTATION

internal gs_file_handler CreatePlatformFileHandler(gs_allocator A);
internal void CleanupPlatformFileHandler(gs_file_handler* H, gs_allocator A);

# if PLATFORM_WINDOWS
#  include "./gs_file_win32.h"
# elif PLATFORM_OSX
#  include "./gs_file_osx.h"
# elif PLATFORM_LINUX
#  include "./gs_file_linux.h"
# endif

#endif

#endif //GS_FILE_H
