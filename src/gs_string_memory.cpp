static int32_t
CalculateSlotCountFromSize (int32_t RequestedSize, int32_t SlotSize)
{
    int32_t SlotCount = RequestedSize / SlotSize;
    if (SlotCount * SlotSize < RequestedSize)
    {
        SlotCount += 1;
    }
    return SlotCount;
}

static bool
SlotsAreContiguous (slot_header* First, slot_header* Second)
{
    bool Result = false;
    uint8_t* FirstSlotNextAddress = (uint8_t*)First + First->Size;
    uint8_t* SecondAddress = (uint8_t*)Second;
    Result = FirstSlotNextAddress == SecondAddress;
    return Result;
}

static contiguous_slot_count_result
CountContiguousSlots (slot_header* First)
{
    Assert(First != 0);
    
    contiguous_slot_count_result Result = {};
    Result.Count = 1;
    
    slot_header* IterPrev = First;
    slot_header* Iter = First->Next;
    while (Iter && SlotsAreContiguous(IterPrev, Iter))
    {
        Result.Count++;
        IterPrev = Iter;
        Iter = Iter->Next;
    }
    
    Result.LastContiguousSlot = IterPrev;
    return Result;
}

static slot_header*
GetSlotAtOffset(slot_header* First, int32_t Offset)
{
    slot_header* Iter = First;
    int32_t Count = 0;
    while (Count < Offset && Iter)
    {
        Iter = Iter->Next;
        Count++;
    }
    return Iter;
}

static slot_header*
InsertSlotIntoList (slot_header* NewSlot, slot_header* ListStart)
{
    slot_header* List = ListStart;
    if (NewSlot < List)
    {
        NewSlot->Next = List;
        List = NewSlot;
    }
    else
    {
        slot_header* PrevIter = List;
        slot_header* Iter = List->Next;
        while (Iter && NewSlot > Iter)
        {
            PrevIter = Iter;
            Iter = Iter->Next;
        }
        
        Assert(PrevIter);
        if (PrevIter) 
        { 
            PrevIter->Next = NewSlot;
        }
        
        if (Iter)
        {
            NewSlot->Next = Iter;
        }
    }
    return List;
}

static void
ZeroMemory (uint8_t* Memory, int32_t Size)
{
    for (int i = 0; i < Size; i++) { Memory[i] = 0; }
}

static void
AllocStringFromStringArena (string* String, int32_t Size, slot_arena* Storage)
{
    int32_t SlotCount = CalculateSlotCountFromSize(Size, Storage->SlotSize);
    slot_header* Slot = Storage->FreeList;
    slot_header* PrevSlot = 0;
    while (Slot)
    {
        contiguous_slot_count_result ContiguousSlots = CountContiguousSlots(Slot);
        if (ContiguousSlots.Count >= SlotCount)
        {
            slot_header* NextStartSlot = GetSlotAtOffset(Slot, SlotCount);
            if (PrevSlot)
            {
                PrevSlot->Next = NextStartSlot;
            }
            else
            {
                Storage->FreeList = NextStartSlot;
            }
            break;
        }
        else
        {
            PrevSlot = Slot;
            Slot = Slot->Next; 
        }
    }
    
    if (Slot)
    {
        String->Data = (char*)Slot;
        //ZeroMemory(Result, SlotCount * Storage->SlotSize);
        
        String->Max = SlotCount * Storage->SlotSize;
        String->Length = 0;
    }
}

static string
AllocStringFromStringArena (int32_t Size, slot_arena* Storage)
{
    string Result = {0};
    AllocStringFromStringArena(&Result, Size, Storage);
    return Result;
}

static void
FreeToStringArena (string* String, slot_arena* Storage)
{
    uint8_t* Base = (uint8_t*)(String->Data);
    uint8_t* End = Base + String->Max - 1;
    uint8_t* MemoryEnd = Storage->Memory + (Storage->SlotSize * Storage->SlotCount);
    Assert((Base >= Storage->Memory) && (End < MemoryEnd));
    Assert((String->Max % Storage->SlotSize) == 0);
    
    int32_t SizeReclaimed = 0;
    slot_header* Slot = (slot_header*)Base;
    while (SizeReclaimed < String->Max)
    {
        Slot->Size = Storage->SlotSize;
        Storage->FreeList = InsertSlotIntoList(Slot, Storage->FreeList);
        SizeReclaimed += Storage->SlotSize;
        Slot = (slot_header*)(Base + SizeReclaimed);
    }
    
    String->Data = 0;
    String->Length = 0;
    String->Max = 0;
}

static void
ReallocFromStringArena (string* String, int32_t NewSize, slot_arena* Storage)
{
    string NewString = AllocStringFromStringArena(NewSize, Storage);
    CopyStringTo(*String, &NewString);
    FreeToStringArena(String, Storage);
    *String = NewString;
}