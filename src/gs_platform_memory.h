//
// File: gs_memory.h
// Author: Peter Slattery
// Creation Date: 2020-03-30
//
// Usage:
// To use, include this file in ONE file in your program, and define one of values
// listed below, based on which implementation you'd like to use
//
// Defines
// GS_MEMORY_CSTDLIB - includes the C standard libary implementation.
// Depends on: <stdlib.h>
//
// GS_MEMORY_WIN32
// Depends on: <windows.h>
//
// GS_MEMORY_OSX
// Depends on: <stdlib.h> ?? Not sure about this, Might actually depend on <Cocoa/Cocoa.h>
//
#ifndef GS_MEMORY_H

#ifndef GS_TYPES_H
# error "gs_memory.h relies on gs_types.h. Please include gs_types.h before including gs_memory.h"
#endif

#ifdef GS_MEMORY_CSTDLIB

static void*
gs_Alloc(u64 Size)
{
    void* Result = malloc(Size);
    return Result;
}

static void
gs_Free(void* Ptr, int Size)
{
    free(Base);
}

#endif

#if defined(GS_MEMORY_WIN32) || defined(GS_PLATFORM_WIN32)

static void*
gs_Alloc(u64 Size)
{
    void* Result = (void*)VirtualAlloc(NULL, Size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    return Result;
}

static void
gs_Free(void* Base, u64 Size)
{
    bool Result = VirtualFree(Base, 0, MEM_RELEASE);
    if (!Result)
    {
        int Error = GetLastError();
        // TODO(Peter): I'm waiting to see an error actually occur here
        // to know what it could possibly
    }
}

#endif

#if defined(GS_MEMORY_OSX) || defined(GS_PLATFORM_OSX)

static void*
gs_Alloc(u64 Size)
{
	void* Result = 0;
	char* StartAddress = (char*)0;
	int Prot = PROT_READ | PROT_WRITE;
	int Flags = MAP_PRIVATE | MAP_ANON;
	Result = (void*)mmap(StartAddress, Size, Prot, Flags, -1, 0);
	return Result;
}

static void
gs_Free(unsigned char* Base, u64 Size)
{
	munmap((void*)Base, (size_t)Size);
}

#endif


#define GS_MEMORY_H
#endif // GS_MEMORY_H