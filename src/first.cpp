#include <stdlib.h>
#include <stdint.h>

#include "gs_string.h"
#include "gs_string.cpp"
#include "gs_string_memory.h"
#include "gs_string_memory.cpp"

slot_arena StringArena = {};

void PrintChars (string* String, int32_t Count)
{
    char* Iter = String->Data;
    for (int i = 0; i < Count; i++)
    {
        *Iter++ = (char)('A' + i);
    }
    String->Length = Count;
}

int main (int ArgCount, char* Args[])
{
    ////////////////////////////////////////////////////////////////
    //    Char Functions
    
    char ForwardArray[] = "Hello, Sailor";
    char BackwardArray[] = "roliaS ,olleH";
    Assert(CharArraysEqual(ForwardArray, 13, ForwardArray, 13));
    Assert(!CharArraysEqual(ForwardArray, 13, BackwardArray, 13));
    
    ReverseCharArray(ForwardArray, 13);
    Assert(CharArraysEqual(ForwardArray, 13, BackwardArray, 13));
    
    char UIntString[] = "1234";
    uint32_t UIntValue = 1234;
    uint32_t ParsedUInt = ParseUnsignedInt(UIntString, 4);
    Assert(ParsedUInt == UIntValue);
    char StringifiedUInt[4] = {};
    UIntToString(UIntValue, StringifiedUInt, 4);
    Assert(CharArraysEqual(UIntString, 4, StringifiedUInt, 4));
    
    char IntString[] = "-1234";
    int32_t IntValue = -1234;
    int32_t ParsedInt = ParseSignedInt(IntString, 5);
    Assert(ParsedInt == IntValue);
    char StringifiedInt[5] = {};
    IntToString(IntValue, StringifiedInt, 5);
    Assert(CharArraysEqual(IntString, 5, StringifiedInt, 5));
    
    char FloatString[] = "-1234.125";
    float FloatValue = -1234.125f;
    float ParsedFloat = ParseFloat(FloatString, 8);
    Assert(ParsedFloat == FloatValue);
    char StringifiedFloat[10] = {};
    FloatToString(FloatValue, StringifiedFloat, 10, 3);
    Assert(CharArraysEqual(FloatString, 8, StringifiedFloat, 8));
    
    ////////////////////////////////////////////////////////////////
    //
    
    StringArena.SlotSize = 256;
    StringArena.SlotCount = 32;
    StringArena.Memory = (uint8_t*)malloc(StringArena.SlotSize * StringArena.SlotCount);
    slot_header* PrevSlotHeader = 0;
    for (int i = StringArena.SlotCount - 1; i >= 0; i--)
    {
        uint8_t* SlotBase = StringArena.Memory + (i * StringArena.SlotSize);
        slot_header* SlotHeader = (slot_header*)SlotBase;
        SlotHeader->Size = StringArena.SlotSize;
        SlotHeader->Next = PrevSlotHeader;
        
        // TEST(peter): Should be true always, except on the first iteration, when there is no next slot
        bool Contiguity = SlotsAreContiguous(SlotHeader, PrevSlotHeader);
        Assert(Contiguity || SlotHeader->Next == 0);
        
        PrevSlotHeader = SlotHeader;
    }
    StringArena.FreeList = PrevSlotHeader;
    
    // TEST(peter): Count Should equal StringArena.SlotCount
    int32_t ContiguousSlotsCountBefore = CountContiguousSlots(StringArena.FreeList).Count;
    Assert(ContiguousSlotsCountBefore == StringArena.SlotCount);
    
    // TEST(peter): Should be false
    bool Contiguity = SlotsAreContiguous(StringArena.FreeList, StringArena.FreeList->Next->Next);
    Contiguity = SlotsAreContiguous(StringArena.FreeList->Next->Next, StringArena.FreeList);
    Assert(!Contiguity);
    
    int32_t Slots = CalculateSlotCountFromSize(10, 256);
    Assert(Slots == 1);
    Slots = CalculateSlotCountFromSize(256, 256);
    Assert(Slots == 1);
    Slots = CalculateSlotCountFromSize(345, 256);
    Assert(Slots == 2);
    Slots = CalculateSlotCountFromSize(1024, 256);
    Assert(Slots == 4);
    
    slot_header* HeaderTen = GetSlotAtOffset(StringArena.FreeList, 10);
    slot_header* HeaderThree = GetSlotAtOffset(StringArena.FreeList, 3);
    slot_header* HeaderFive = GetSlotAtOffset(StringArena.FreeList, 5);
    
    string StringA = AllocStringFromStringArena(10, &StringArena);
    string StringB = AllocStringFromStringArena(345, &StringArena);
    
#if 0
    // TEST(peter): Should Assert
    uint8_t* RandomMemory = (uint8_t*)malloc(256);
    string RandomMemString = {};
    RandomMemString.Data = (char*)RandomMemory;
    RandomMemString.Max = 256;
    FreeToStringArena(&RandomMemString, &StringArena); 
#endif
    FreeToStringArena(&StringA, &StringArena);
    FreeToStringArena(&StringB, &StringArena);
    // TEST(peter): After freeing both allocations, ContiguousSlotCountBefore and ContiguousSlotCountAfter should be equal
    int32_t ContiguousSlotCountAfter = CountContiguousSlots(StringArena.FreeList).Count;
    Assert(ContiguousSlotCountAfter == ContiguousSlotsCountBefore);
    
    // TEST(peter): Set up a free list where the first element is too small, so it has to traverse to find an appropriately
    // sized block
    // The slots will look list [256][used][256][256][256] etc..
    StringA = AllocStringFromStringArena(256, &StringArena);
    StringB = AllocStringFromStringArena(256, &StringArena);
    FreeToStringArena(&StringA, &StringArena);
    uint32_t Contiguous = CountContiguousSlots(StringArena.FreeList).Count; // Should = 1;
    string StringC = AllocStringFromStringArena(512, &StringArena);
    slot_header* HeaderC = (slot_header*)(StringC.Data);
    
    string ReallocTestString = AllocStringFromStringArena(256, &StringArena);
    PrintChars(&ReallocTestString, 24);
    ReallocFromStringArena(&ReallocTestString, 512, &StringArena);
    
    
    string TestString = AllocStringFromStringArena(10, &StringArena);
    PrintChars(&TestString, TestString.Max);
    ReallocFromStringArena(&TestString, 20, &StringArena);
    PrintChars(&TestString, TestString.Max);
    ReallocFromStringArena(&TestString, 10, &StringArena);
    FreeToStringArena(&TestString, &StringArena);
    
    string EqualityStringA = AllocStringFromStringArena(345, &StringArena);
    string EqualityStringB = AllocStringFromStringArena(415, &StringArena);
    // Equality should succeed despite length differences
    string EqualityStringC = AllocStringFromStringArena(256, &StringArena); 
    string EqualityStringD = AllocStringFromStringArena(256, &StringArena); // Equality should fail
    string EqualityStringEmpty = {};
    
    PrintChars(&EqualityStringA, 24);
    PrintChars(&EqualityStringB, 24);
    PrintChars(&EqualityStringC, 24);
    PrintChars(&EqualityStringD, 12);
    
    bool ABEquality = StringsEqual(EqualityStringA, EqualityStringB); // Should Succeed
    bool ACEquality = StringsEqual(EqualityStringA, EqualityStringC); // Should Succeed
    bool ADEquality = StringsEqual(EqualityStringA, EqualityStringD); // Should Fail
    bool AEEquality = StringsEqual(EqualityStringA, EqualityStringEmpty); // Should Fail
    
    Assert(ABEquality);
    Assert(ACEquality);
    Assert(!ADEquality);
    Assert(!AEEquality);
    
    return 0;
}