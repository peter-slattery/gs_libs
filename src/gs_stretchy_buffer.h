/* date = August 22nd 2021 2:17 pm */

#ifndef GS_STRETCHY_BUFFER_H
#define GS_STRETCHY_BUFFER_H

typedef struct
{
  u64 EleSize;
  u64 Cap;
  u64 Len;
  gs_allocator A;
} gs_stretchy_buf_header;

#define SBufCreate(type, init_cap, alloc) (type*)SBufCreate_(sizeof(type), (init_cap), (alloc))
internal u8*
SBufCreate_(u64 EleSize, u64 InitCap, gs_allocator A)
{
  // TODO(PS): align to 8 bit boundary?
  u64 InitSize = (EleSize * InitCap) + sizeof(gs_stretchy_buf_header);
  u8* Mem = Alloc(A, InitSize, "SBufCreate");
  
  gs_stretchy_buf_header* Header = (gs_stretchy_buf_header*)Mem;
  *Header = (gs_stretchy_buf_header){
    .EleSize = EleSize,
    .Cap = InitCap,
    .Len = 0,
    .A = A
  };
  
  u8* ArrBase = (u8*)(Header + 1);
  return ArrBase;
}

#define SBufGetHeader(arr_base) ((gs_stretchy_buf_header*)(arr_base) - 1)

#define SBufLen(arr_base) SBufLen_((u8*)(arr_base))
internal u64
SBufLen_(u8* ArrBase)
{
  gs_stretchy_buf_header* Header = SBufGetHeader(ArrBase);
  return Header->Len;
}

#define SBufCap(arr_base) SBufCap_((u8*)(arr_base))
internal u64
SBufCap_(u8* ArrBase)
{
  gs_stretchy_buf_header* Header = SBufGetHeader(ArrBase);
  return Header->Cap;
}

internal u8*
SBufGrowIfAtCap(u8* ArrBase)
{
  u8* NewBase = ArrBase;
  gs_stretchy_buf_header* Header = SBufGetHeader(ArrBase);
  if ((Header->Len + 1) > Header->Cap)
  {
    u64 CurrArrSize = Header->Cap * Header->EleSize;
    u64 CurrSize = sizeof(gs_stretchy_buf_header) + CurrArrSize;
    
    u64 NewCap = Header->Cap * 2;
    u64 NewArrSize = NewCap * Header->EleSize; 
    u64 NewSize = sizeof(gs_stretchy_buf_header) + NewArrSize;
    
    u8* HBase = Realloc(Header->A, Header, CurrSize, NewSize);
    
    Header = (gs_stretchy_buf_header*)HBase;
    Header->Cap = NewCap;
    NewBase = (u8*)(Header + 1);
  }
  return NewBase;
}

internal void
SBufPushBack(u8* ArrBase, u64 Count)
{
  gs_stretchy_buf_header* Header = SBufGetHeader(ArrBase);
  Assert(((Header->Len - 1) + Count) < Header->Cap);
  u64 StartOffset = Header->Len * Header->EleSize;
  for (u64 i = StartOffset; i > 0; i--)
  {
    ArrBase[i] = ArrBase[i - 1];
  }
  Header->Len += 1;
}

#define SBufPrepend(arr_base, val) \
(arr_base) = SBufGrowIfAtCap(arr_base); \
SBufPushBack(arr_base, 1); \
(arr_base)[0] = (val)

#define SBufAppend(arr_base, val) \
(arr_base) = SBufGrowIfAtCap(arr_base); \
(arr_base)[SBufGetHeader(arr_base)->Len++] = (val)


#endif //GS_STRETCHY_BUFFER_H
