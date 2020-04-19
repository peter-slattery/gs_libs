/*
gs_radix_sort.h - An implementation of radix sort for fixed size unsigned 32bit integer buffers

TODO
*/

#ifndef GS_RADIX_SORT_H

#ifndef GS_TYPES_H
# error "gs_radix_sort.h relies on gs_types.h. Please include gs_types.h before gs_radix_sort."
#endif

struct gs_radix_entry
{
    u32 Radix;
    u32 ID;
};

static void
RadixSortInPlace_ (gs_radix_entry* Data, u32 Start, u32 End, u32 Iteration)
{
    u32 Shift = Iteration;
    u32 ZerosBoundary = Start;
    u32 OnesBoundary = End - 1;
    
    for (u32 d = Start; d < End; d++)
    {
        gs_radix_entry Entry = Data[ZerosBoundary];
        u32 Place = (Entry.Radix >> Shift) & 0x1;
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
RadixSortInPlace (gs_radix_entry* Data, u32 Count)
{
    u32 Highest = 0;
    for (u32 i = 0; i < Count; i++)
    {
        if (Data[i].Radix > Highest)
        {
            Highest = Data[i].Radix;
        }
    }
    
    u32 Iterations = 0;
    while (Highest > 1)
    {
        ++Iterations;
        Highest = Highest >> 1;
    }
    
    RadixSortInPlace_(Data, 0, Count, Iterations);
}

#define GS_RADIX_SORT_H
#endif // GS_RADIX_SORT_H