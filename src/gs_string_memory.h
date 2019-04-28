struct slot_header
{
    slot_header* Next;
    int32_t Size;
    int32_t TestValue;
};

struct slot_arena
{
    uint8_t* Memory;
    int32_t SlotSize;
    int32_t SlotCount;
    slot_header* FreeList;
};

struct contiguous_slot_count_result
{
    int32_t Count;
    slot_header* LastContiguousSlot;
};
