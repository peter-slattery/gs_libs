/* date = August 25th 2021 0:32 am */

#ifndef GS_MEMORY_EMCC_H
#define GS_MEMORY_EMCC_H

PLATFORM_ALLOC(AllocEmcc)
{
  u8* Result = (u8*)malloc(Size);
  if (ResultSize) *ResultSize = Size;
  return Result;
}

PLATFORM_REALLOC(ReallocEmcc)
{
  u8* Result = (u8*)realloc(Base, NewSize);
  return Result;
}

PLATFORM_FREE(FreeEmcc)
{
  free(Base);
}

internal gs_allocator 
CreatePlatformAllocator()
{
  return AllocatorCreate(AllocEmcc, ReallocEmcc, FreeEmcc, 0);
}

#endif //GS_MEMORY_EMCC_H
