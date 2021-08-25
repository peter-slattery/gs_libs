/* date = August 25th 2021 10:08 am */

#ifndef GS_HASH_H
#define GS_HASH_H

//
// DJB2
// Source: http://www.cse.yorku.ca/~oz/hash.html
internal u64
HashDJB2_ToU64(char* Base, u64 Len)
{
  // Source: DJB2 Hash
  u64 Hash = 5381;
  for (u32 i = 0; i < Len; i++)
  {
    Hash = ((Hash << 5) + Hash) + (u8)Base[i];
  }
  return Hash;
}
internal u64
HashDJB2_CStrToU64(char* S)
{
  u64 Len = CStringLength(S);
  return HashDJB2_ToU64(S, Len);
}

#if defined(GS_STRING_H)

internal u64
HashDJB2_StrToU64(gs_string S)
{
  return HashDJB2_ToU64((char*)S.Data, S.Len);
}

#endif // GS_STRING_H

//
// SHA-1
// Publication Src:
// Code Reference Src: https://www.packetizer.com/security/sha1/

typedef struct
{
  u32 MsgDigest[5];
  
  u32 MsgLenLo;
  u32 MsgLenHi;
  
  u8 MsgBlock[64];
  u64 MsgBlockIndex;
  
  s32 Computed;
  s32 Corrupted;
} gs_sha1_ctx;

internal void
SHA1Reset(gs_sha1_ctx* Ctx)
{
  Ctx->MsgLenLo = 0;
  Ctx->MsgLenHi = 0;
  Ctx->MsgBlockIndex = 0;
  
  Ctx->MsgDigest[0] = 0x67452301;
  Ctx->MsgDigest[1] = 0xEFCDAB89;
  Ctx->MsgDigest[2] = 0x98BADCFE;
  Ctx->MsgDigest[3] = 0x10325476;
  Ctx->MsgDigest[4] = 0xC3D2E1F0;
  
  Ctx->Computed = 0;
  Ctx->Corrupted = 0;
}

#define SHA1CircularShift(bits,word) \
((((word) << (bits)) & 0xFFFFFFFF) | \
((word) >> (32-(bits))))

// names here are the same as in the publication
// though types may not be, in an effort to be more
// clear
internal void
SHA1ProcessMsgBlock(gs_sha1_ctx* Ctx)
{
  u32 K[] = 
  {
    0x5A827999,
    0x6ED9EBA1,
    0x8F1BBCDC,
    0xCA62C1D6
  };
  s32 t;             // loop counter
  u32 temp;          
  u32 W[80];         // word sequence
  u32 A, B, C, D, E; // word buffers
  
  // init first 16 words
  for (t = 0; t < 16; t++)
  {
    W[t]  = ((u32)Ctx->MsgBlock[t * 4])     << 24;
    W[t] |= ((u32)Ctx->MsgBlock[t * 4 + 1]) << 16;
    W[t] |= ((u32)Ctx->MsgBlock[t * 4 + 2]) << 8;
    W[t] |= ((u32)Ctx->MsgBlock[t * 4 + 3]);
  }
  
  for (t = 16; t < 80; t++)
  {
    W[t] = SHA1CircularShift(1, 
                             W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
  }
  
  A = Ctx->MsgDigest[0];
  B= Ctx->MsgDigest[1];
  C= Ctx->MsgDigest[2];
  D= Ctx->MsgDigest[3];
  E= Ctx->MsgDigest[4];
  
  for (t = 0; t < 20; t++)
  {
    temp = SHA1CircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
    temp &= 0xFFFFFFFF;
    E = D;
    D = C;
    C = SHA1CircularShift(30,B);
    B = A;
    A = temp;
  }
  
  for(t = 20; t < 40; t++)
  {
    temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
    temp &= 0xFFFFFFFF;
    E = D;
    D = C;
    C = SHA1CircularShift(30,B);
    B = A;
    A = temp;
  }
  
  for(t = 40; t < 60; t++)
  {
    temp = SHA1CircularShift(5,A) +
      ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
    temp &= 0xFFFFFFFF;
    E = D;
    D = C;
    C = SHA1CircularShift(30,B);
    B = A;
    A = temp;
  }
  
  for(t = 60; t < 80; t++)
  {
    temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
    temp &= 0xFFFFFFFF;
    E = D;
    D = C;
    C = SHA1CircularShift(30,B);
    B = A;
    A = temp;
  }
  
  Ctx->MsgDigest[0] = (Ctx->MsgDigest[0] + A) & 0xFFFFFFFF;
  Ctx->MsgDigest[1] = (Ctx->MsgDigest[1] + B) & 0xFFFFFFFF;
  Ctx->MsgDigest[2] = (Ctx->MsgDigest[2] + C) & 0xFFFFFFFF;
  Ctx->MsgDigest[3] = (Ctx->MsgDigest[3] + D) & 0xFFFFFFFF;
  Ctx->MsgDigest[4] = (Ctx->MsgDigest[4] + E) & 0xFFFFFFFF;
  
  Ctx->MsgBlockIndex = 0;
}

internal void
SHA1PadMsg(gs_sha1_ctx* Ctx)
{
  if (Ctx->MsgBlockIndex > 55)
  {
    Ctx->MsgBlock[Ctx->MsgBlockIndex++] = 0x80;
    while (Ctx->MsgBlockIndex < 64)
    {
      Ctx->MsgBlock[Ctx->MsgBlockIndex++] = 0;
    }
    SHA1ProcessMsgBlock(Ctx);
    while (Ctx->MsgBlockIndex < 56)
    {
      Ctx->MsgBlock[Ctx->MsgBlockIndex++] = 0;
    }
  }
  else
  {
    Ctx->MsgBlock[Ctx->MsgBlockIndex++] = 0x80;
    while (Ctx->MsgBlockIndex < 56)
    {
      Ctx->MsgBlock[Ctx->MsgBlockIndex++] = 0;
    }
  }
  
  Ctx->MsgBlock[56] = (Ctx->MsgLenHi >> 24) & 0xFF;
  Ctx->MsgBlock[57] = (Ctx->MsgLenHi >> 16) & 0xFF;
  Ctx->MsgBlock[58] = (Ctx->MsgLenHi >>  8) & 0xFF;
  Ctx->MsgBlock[59] = (Ctx->MsgLenHi)       & 0xFF;
  Ctx->MsgBlock[60] = (Ctx->MsgLenLo >> 24) & 0xFF;
  Ctx->MsgBlock[61] = (Ctx->MsgLenLo >> 16) & 0xFF;
  Ctx->MsgBlock[62] = (Ctx->MsgLenLo >>  8) & 0xFF;
  Ctx->MsgBlock[63] = (Ctx->MsgLenLo)       & 0xFF;
  
  SHA1ProcessMsgBlock(Ctx);
}

internal s32
SHA1Result(gs_sha1_ctx* Ctx)
{
  if (Ctx->Corrupted) return 0;
  if (!Ctx->Computed)
  {
    SHA1PadMsg(Ctx);
    Ctx->Computed = 1;
  }
  return 1;
}

internal void
SHA1Input(gs_sha1_ctx* Ctx, u8* Data, u32 Len)
{
  if (!Len) return;
  if (Ctx->Computed || Ctx->Corrupted) {
    Ctx->Corrupted = 1;
    return;
  }
  
  while (Len-- && !Ctx->Corrupted)
  {
    Ctx->MsgBlock[Ctx->MsgBlockIndex++] = (*Data & 0xFF);
    Ctx->MsgLenLo += 8;
    Ctx->MsgLenLo &= 0xFFFFFFFF;
    if (Ctx->MsgLenLo == 0)
    {
      Ctx->MsgLenHi++;
      Ctx->MsgLenHi &= 0xFFFFFFFF;
      if (Ctx->MsgLenHi == 0)
      {
        Ctx->Corrupted = 1;
      }
    }
    
    if (Ctx->MsgBlockIndex == 64)
    {
      SHA1ProcessMsgBlock(Ctx);
    }
    Data++;
  }
}

#endif //GS_HASH_H
