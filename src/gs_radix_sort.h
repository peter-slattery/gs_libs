/*
gs_radix_sort.h - An implementation of radix sort for fixed size unsigned 32bit integer buffers

TODO
 x remove ALL dependencies - currently rely on stdlib.h
 x support sorting in bases other than 10
 x in place, binary, Most significant digit sort

*/

#ifndef GS_RADIX_SORT_H

#ifdef DEBUG
#if !defined(GSRad_Assert)
#define GSRad_Assert(expression) \
if(!(expression)) { \
*((int *)0) = 5; \
}
#endif // !defined(GSRad_Assert)
#endif // DEBUG

typedef unsigned int gs_rad_u32;
typedef unsigned int gs_rad_b32;

struct gs_radix_entry
{
    gs_rad_u32 Radix;
    gs_rad_u32 ID;
};

#define RADIX_SORT_BUCKET_DEPTH 32
struct radix_sort_bucket_buffer
{
    gs_radix_entry Contents[RADIX_SORT_BUCKET_DEPTH];
};

struct radix_sort_bucket
{
    radix_sort_bucket_buffer** Buffers;
    gs_rad_u32 BuffersCount;
    gs_rad_u32 EntriesUsed;
};

struct radix_sort
{
    gs_radix_entry* Data;
    gs_rad_u32 Length;
    gs_rad_u32 Highest;
    gs_rad_u32 TotalIterations;
    gs_rad_u32 CurrentIteration;
};

static void
RadixSortInPlace_ (gs_radix_entry* Data, gs_rad_u32 Start, gs_rad_u32 End, gs_rad_u32 Iteration)
{
    gs_rad_u32 Shift = Iteration;
    gs_rad_u32 ZerosBoundary = Start;
    gs_rad_u32 OnesBoundary = End - 1;
    
    for (gs_rad_u32 d = Start; d < End; d++)
    {
        gs_radix_entry Entry = Data[ZerosBoundary];
        gs_rad_u32 Place = (Entry.Radix >> Shift) & 0x1;
        if (Place)
        {
            gs_radix_entry Evicted = Data[OnesBoundary];
            Data[OnesBoundary] = Entry;
            Data[ZerosBoundary] = Evicted;
            OnesBoundary -= 1;
        }
        else
        {
            ZerosBoundary += 1;
        }
    }
    
    if (Iteration > 0)
    {
        RadixSortInPlace_(Data, Start, ZerosBoundary, Iteration - 1);
        RadixSortInPlace_(Data, ZerosBoundary, End, Iteration - 1);
    }
}

static void
RadixSortInPlace (gs_radix_entry* Data, gs_rad_u32 Count)
{
    gs_rad_u32 Highest = 0;
    for (gs_rad_u32 i = 0; i < Count; i++)
    {
        if (Data[i].Radix > Highest)
        {
            Highest = Data[i].Radix;
        }
    }
    
    gs_rad_u32 Iterations = 0;
    while (Highest > 1)
    {
        ++Iterations;
        Highest = Highest >> 1;
    }
    
    RadixSortInPlace_(Data, 0, Count, Iterations);
}

#define GS_RADIX_SORT_H
#endif // GS_RADIX_SORT_H