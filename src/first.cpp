#include "gs_language.h"

#include "gs_string.h"
#include "gs_vector_matrix.h"
#include "gs_memory.h"

/*
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <intrin.h>
*/

int CALLBACK
WinMain (HINSTANCE HInstance,
         HINSTANCE HPrevInstance,
         LPSTR CommandLine, 
         int ShowCode)
{
    TestVectorMatrixMultiplication();
    TestStrings();
    
    heap_memory_arena Arena = {};
    InitHeapMemoryArena(&Arena, Allocate(Kilobytes(32)), Kilobytes(32));
    
    u8* A = PushSize_(&Arena, 12);
    u8* B = PushSize_(&Arena, 32);
    u8* C = PushSize_(&Arena, 64);
    u8* D = PushSize_(&Arena, 16);
    
    return 0;
}

void WinMainCRTStartup()
{
    int Result = WinMain(GetModuleHandle(0), 0, 0, 0);
    ExitProcess(Result);
}