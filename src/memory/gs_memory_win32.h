/* date = May 10th 2021 11:48 pm */

#ifndef GS_MEMORY_WIN32_H
#define GS_MEMORY_WIN32_H

PLATFORM_ALLOC(AllocWin32)
{
  u8* Result = (u8*)VirtualAlloc(NULL, Size,
                                 MEM_COMMIT | MEM_RESERVE,
                                 PAGE_EXECUTE_READWRITE);
  if (ResultSize) *ResultSize = Size;
  return Result;
}

PLATFORM_REALLOC(ReallocWin32)
{
  u8* Result = (u8*)VirtualAlloc(NULL, NewSize,
                                 MEM_COMMIT | MEM_RESERVE,
                                 PAGE_EXECUTE_READWRITE);
  CopyMemoryTo(Base, Result, OldSize);
  VirtualFree(Base, 0, MEM_RELEASE);
  return Result;
}

PLATFORM_FREE(FreeWin32)
{
  VirtualFree(Base, 0, MEM_RELEASE);
}

internal gs_allocator 
CreatePlatformAllocator()
{
  return AllocatorCreate(AllocWin32, ReallocWin32, FreeWin32, 0);
}

#endif //GS_MEMORY_WIN32_H
