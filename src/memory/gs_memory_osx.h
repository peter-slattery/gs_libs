#ifndef GS_MEMORY_OSX_H
#define GS_MEMORY_OSX_H

#include <stdlib.h>

PLATFORM_ALLOC(AllocOSX)
{
	u8* Result = (u8*)malloc(Size);
	if (ResultSize) *ResultSize = Size;
	return Result;
}

PLATFORM_REALLOC(ReallocOSX)
{
	u8* Result = (u8*)realloc(Base, Size);
	return Result;
}

PLATFORM_FREE(FreeOSX)
{
	free(Base);
}

internal gs_allocator
CreatePlatformAllocator()
{
	return AllocatorCreate(AllocOSX, ReallocOSX, FreeOSX, 0);
}

#endif