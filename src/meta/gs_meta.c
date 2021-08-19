#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <gl/gl.h>

#define GS_PLATFORM_IMPLEMENTATION
#include "../gs_platform.h"
#include "../gs_types.h"
#define ASSERTS_ACTIVE 1
#include "../gs_assert.h"

#define GS_GFX_PROFILE_SCOPE PROFILE_SCOPE_G
#include "../gs_profiler.h"

#define GS_MEMORY_PROFILE_FUNC PROFILE_SCOPE_G
#define GS_FILE_PROFILE_FUNC PROFILE_SCOPE_G
#define GS_THREAD_PROFILE_FUNC PROFILE_SCOPE_G
#define GS_MATH_PROFILE_FUNC PROFILE_SCOPE_G
#include "../memory/gs_memory.h"
#include "../file/gs_file.h"
#include "../thread/gs_thread.h"
#include "../time/gs_time.h"
#include "../gs_math.h"
#include "../gs_printf.h"

#include "../gs_profiler.c"

typedef enum gs_meta_token_kind
{
  MetaToken_FirstNonCharKind = 128,
  
  MetaToken_BitShiftDown,
  MetaToken_BitShiftUp,
  
  MetaToken_Equals,
  MetaToken_NotEquals,
  
  MetaToken_AddAssign,
  MetaToken_SubAssign,
  MetaToken_MulAssign,
  MetaToken_DivAssign,
  MetaToken_ModAssign,
  MetaToken_BitShiftDownAssign,
  MetaToken_BitShiftUpAssign,
  
  MetaToken_Or,
  MetaToken_And,
  
  MetaToken_Identifier,
  
  MetaToken_NumberDecimal,
  MetaToken_NumberHexadecimal,
  MetaToken_NumberOctal,
  MetaToken_NumberBinary,
  
  MetaToken_Float,
  MetaToken_Double,
  
  MetaToken_Count,
} gs_meta_token_kind;

typedef struct gs_meta_token
{
  char* InternedString;
  gs_meta_token_kind Kind;
  struct gs_meta_token* Next;
} gs_meta_token;

typedef struct gs_meta_tokenizer
{
  char* Base;
  u64 Size;
  
  char* At;
  u64 Column;
  u64 Row;
  
  char** InternedStrings;
  u64 InternedStringsCount;
  u64 InternedStringsCountMax;
  
  gs_meta_token* TokensRoot;
  gs_meta_token* TokensHead;
  
  gs_memory_arena* Arena;
} gs_meta_tokenizer;

internal bool
StringsEqual(char* A, char* B, u64 Len)
{
  bool Result = true;
  for (u64 i = 0; i < Len; i++)
  {
    if (A[i] != B[i])
    {
      Result = false;
      break;
    }
  }
  return Result;
}

internal char*
GetInternedString(gs_meta_tokenizer* T, char* Start, char* End)
{
  char* Result = 0;
  u64 Len = (u64)(End - Start);
  
  for (u64 i = 0; i < T->InternedStringsCount; i++)
  {
    char* At = T->InternedStrings[i];
    if (StringsEqual(Start, At, Len))
    {
      Result = At;
      break;
    }
  }
  
  if (!Result)
  {
    Assert(T->InternedStringsCount < T->InternedStringsCountMax);
    u64 Index = T->InternedStringsCount++;
    
    Result = PushArray(T->Arena, char, Len + 1);
    CopyMemoryTo((u8*)Start, (u8*)Result, Len);
    Result[Len] = 0;
    
    T->InternedStrings[Index] = Result;
  }
  
  return Result;
}

internal void
TokenizerTokenPush(gs_meta_tokenizer* T, char* Start, char* End, gs_meta_token_kind Kind)
{
  gs_meta_token* Token = PushStruct(T->Arena, gs_meta_token);
  *Token = (gs_meta_token){
    .InternedString = GetInternedString(T, Start, End),
    .Kind = Kind,
  };
  
  if (!T->TokensRoot)
  {
    T->TokensRoot = Token;
    T->TokensHead = Token;
  } else {
    T->TokensHead->Next = Token;
    T->TokensHead = Token;
  }
}

internal bool
TokenizerCanAdvance(gs_meta_tokenizer* T)
{
  bool Result = (u64)(T->At - T->Base) < T->Size;
  return Result;
}

global char* CharTable_Whitespace = " \n\t\r\v";
global char* CharTable_Alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
global char* CharTable_Hex = "0123456789abcdef";
global char* CharTable_Decimal = "0123456789";
global char* CharTable_Octal = "01234567";
global char* CharTable_Binary = "01";

#define SINGLE_CHAR_TOKEN(c) ['c'] = "c"
global char* TokenStringTable[] = {
  ['('] = "(",
  [')'] = ")",
  ['{'] = "{",
  ['}'] = "}",
  ['['] = "[",
  [']'] = "]",
  
  ['!'] = "!",
  ['+'] = "+",
  ['-'] = "-",
  ['*'] = "*",
  ['/'] = "/",
  ['%'] = "%",
  ['='] = "=",
  ['|'] = "|",
  ['&'] = "&",
  ['^'] = "^",
  ['@'] = "@",
  ['#'] = "#",
  ['$'] = "$",
  [','] = ",",
  ['.'] = ".",
  ['?'] = "?",
  [':'] = ":",
  ['\\'] = "\\",
  
  [MetaToken_BitShiftDown] = "<<",
  [MetaToken_BitShiftUp]   = ">>",
  
  [MetaToken_Equals] = "==",
  [MetaToken_NotEquals] = "!=",
  
  [MetaToken_AddAssign] = "+=",
  [MetaToken_SubAssign] = "-=",
  [MetaToken_MulAssign] = "*=",
  [MetaToken_DivAssign] = "/=",
  [MetaToken_ModAssign] = "%=",
  [MetaToken_BitShiftDownAssign] = "<<=",
  [MetaToken_BitShiftUpAssign] = ">>=",
  
  [MetaToken_Or] = "||",
  [MetaToken_And] = "&&",
};
#undef SINGLE_CHAR_TOKEN

internal bool
CharIsInTable(char C, char* Table)
{
  bool Result = false;
  for (char* TAt = Table; *TAt != 0; TAt++)
  {
    if (*TAt == C)
    {
      Result = true;
      break;
    }
  }
  return Result;
}

internal char
CharToLower(char C)
{
  char Result = C;
  if (C >= 'A' && C <= 'Z')
  {
    Result = (C - 'A') + 'a';
  }
  return Result;
}

internal void
Tokenize(gs_meta_tokenizer* T)
{
  while (TokenizerCanAdvance(T))
  {
    while (CharIsInTable(*T->At, CharTable_Whitespace) && TokenizerCanAdvance(T))
    {
      T->At++;
    }
    
    if (CharIsInTable(*T->At, CharTable_Alpha) ||
        *T->At == '_')
    {
      char* Start = T->At;
      while ((CharIsInTable(*T->At, CharTable_Alpha) ||
              CharIsInTable(*T->At, CharTable_Decimal) ||
              *T->At == '_') && 
             TokenizerCanAdvance(T))
      {
        T->At++;
      }
      char* End = T->At;
      //printf("Identifier: %.*s\n", (u32)(IdentifierEnd - IdentifierStart), IdentifierStart);
      TokenizerTokenPush(T, Start, End, MetaToken_Identifier);
    }
    else if (CharIsInTable(*T->At, CharTable_Decimal))
    {
      char* Base = CharTable_Decimal;
      char* Start = T->At;
      gs_meta_token_kind Kind = MetaToken_NumberDecimal;
      if (*T->At == '0')
      {
        T->At++;
        if (*T->At == 'x')
        {
          T->At++;
          Base = CharTable_Hex;
          Kind = MetaToken_NumberHexadecimal;
          Start = T->At;
        }
        else if (*T->At == 'b')
        {
          T->At++;
          Base = CharTable_Binary;
          Kind = MetaToken_NumberBinary;
          Start = T->At;
        }
        else
        {
          Base = CharTable_Octal;
          Kind = MetaToken_NumberOctal;
          Start = T->At;
        }
      }
      
      while (CharIsInTable(CharToLower(*T->At), Base))
      {
        T->At++;
      }
      
      char* End = T->At;
      //printf("Number: %.*s\n", (u32)(NumberEnd - NumberStart), NumberStart);
      TokenizerTokenPush(T, Start, End, Kind);
    }
    else
    {
      char* Start = T->At++;
      char* End = 0;
      gs_meta_token_kind Kind = 0;
      u32 LengthMatched = 0;
      for (u32 i = 0; i < CArrayLength(TokenStringTable); i++)
      {
        if (!TokenStringTable[i]) continue;
        
        bool Matches = true;
        char* SAt = Start;
        char* TAt = TokenStringTable[i];
        
        while (*TAt != 0)
        {
          if (*TAt != *SAt)
          {
            Matches = false;
            break;
          }
          
          SAt++; TAt++;
        }
        
        if (Matches && LengthMatched < (SAt - Start))
        {
          End = SAt;
          LengthMatched = (SAt - Start);
          Kind = i;
        }
      }
      
      if (End)
      {
        TokenizerTokenPush(T, Start, End, Kind);
        //printf("Operator: %.*s\n", (u32)(End - Start), Start);
        T->At = End++;
      }
      else
      {
        printf("Error\n");
      }
    }
  }
}


int main(int ArgCount, char** Args)
{
  gs_allocator Allocator = CreatePlatformAllocator();
  gs_thread_manager ThreadManager = CreatePlatformThreadManager(1);
  gs_time_handler TimeHandler = CreatePlatformTimeHandler();
  ProfilerCreateGlobal(Allocator, 32, 2048, &ThreadManager, TimeHandler);
  
  gs_memory_arena Arena = MemoryArenaCreate(4096, 8, Allocator, 0, 0, "Permanent");
  gs_memory_arena Transient = MemoryArenaCreate(4096, 8, Allocator, 0, 0, "Transient");
  
  gs_file_handler FileHandler = CreatePlatformFileHandler(Allocator);
  PrintFInit(Allocator);
  
  char* Text = "+-! != << <<= *= ({)}Ident12_ _Iden_tifier3 14321 0b01101 0x323AF 012374";
  gs_meta_tokenizer T = {
    .Base = Text,
    .Size = CStringLength(Text),
    .At = Text,
    .Column = 0,
    .Row = 0,
    .Arena = &Arena,
  };
  T.InternedStringsCountMax = 1024;
  T.InternedStrings = PushArray(T.Arena, char*, T.InternedStringsCountMax);
  Tokenize(&T);
  
  for (gs_meta_token* Token = T.TokensRoot; Token != 0; Token = Token->Next)
  {
    switch (Token->Kind)
    {
      case MetaToken_Identifier:
      {
        printf("Identifier - %s\n", Token->InternedString);
      } break;
      
      case MetaToken_NumberOctal:
      {
        printf("Number - 0%s\n", Token->InternedString);
      } break;
      
      case MetaToken_NumberDecimal:
      {
        printf("Number - %s\n", Token->InternedString);
      } break;
      
      case MetaToken_NumberBinary:
      {
        printf("Number - 0b%s\n", Token->InternedString);
      } break;
      
      case MetaToken_NumberHexadecimal:
      {
        printf("Number - 0x%s\n", Token->InternedString);
      } break;
      
      default:
      {
        printf("Operator - %s\n", Token->InternedString);
      } break;
    }
  }
  
  return 0;
}