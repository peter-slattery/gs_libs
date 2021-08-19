/* date = August 17th 2021 0:12 am */

#ifndef GS_TOKENIZER_H
#define GS_TOKENIZER_H

typedef enum {
  CharType_Invalid,
  
  CharType_Alpha,
  CharType_Numeric,
  CharType_AlphaNum,
  CharType_Symbol,
} gs_char_type;

global char* CharTypeStrings[] = {
  "CharType_Invalid",
  
  "CharType_Alpha",
  "CharType_Numeric",
  "CharType_AlphaNum",
  "CharType_Symbol",
};

typedef struct {
  char* Data;
  u64 Len;
  gs_char_type Type;
  
  u64 Col;
  u64 Row;
} gs_token;

typedef struct gs_token_sll gs_token_sll;
struct gs_token_sll {
  gs_token T;
  gs_token_sll* Next;
};

typedef struct {
  char* Base;
  u64 Len;
  u64 At;
  char* CharAt;
  
  gs_token_sll* TokensRoot;
  gs_token_sll* TokensHead;
  
  gs_memory_arena* Arena;
  
  u64 Row;
  u64 Col;
} gs_tokenizer;

//
// Basic Character Identification

internal bool IsWhitespace(char C) { return (C == ' ' || C == '\t'); }
internal bool IsNewline(char C) { return (C == '\n' || C == '\r'); }

internal bool IsAlpha(char C) {
  return ((C >= 'a' && C <= 'z') ||
          (C >= 'A' && C <= 'Z'));
};
internal bool IsNumeric(char C) {
  return (C >= '0' && C <= '9');
};
internal bool IsSymbol(char C) {
  return (C == '!' || C == '@' || 
          C == '#' || C == '$' || 
          C == '%' || C == '^' || 
          C == '&' || C == '*' || 
          C == '(' || C == ')' || 
          C == '-' || C == '+' || 
          C == '=' || C == '-' || 
          C == '{' || C == '}' || 
          C == '[' || C == ']' || 
          C == '"' || C == '\'' || 
          C == ';' || C == ':' || 
          C == ',' || C == '.' || 
          C == '<' || C == '>' || 
          C == '?' || C == '/' || 
          C == '`' || C == '~' || 
          C == '|' || C == '\\');
};

internal gs_char_type
GetCharType(char C)
{
  if (IsAlpha(C))   return CharType_Alpha;
  if (IsNumeric(C)) return CharType_Numeric;
  if (IsSymbol(C))  return CharType_Symbol;
  return CharType_Invalid;
}

//
// Tokenizer

internal gs_tokenizer
TokenizerCreate(char* Base, u64 Len, gs_memory_arena* Arena)
{
  gs_tokenizer T = {};
  T.Base = Base;
  T.Len = Len;
  T.CharAt = T.Base;
  T.Arena = Arena;
  return T;
}

internal void
TokenizerPushToken(gs_tokenizer* T, char* Start, char* End, gs_char_type Type)
{
  gs_token Token = {};
  Token.Data = Start;
  Token.Len = (u64)(End - Start);
  Token.Type = Type;
  
  // NOTE(PS): Tokens get Pushed after the tokenizer has
  // advanced past them. So T->Row is correct since
  // tokens cannot cross line boundaries, but T->Col
  // is at the end of the Token, hence the subtraction
  // of Token.Len
  Token.Row = T->Row;
  Token.Col = T->Col - Token.Len;
  
  gs_token_sll* TokenEle = PushStruct(T->Arena, gs_token_sll);
  TokenEle->T = Token;
  TokenEle->Next = 0;
  
  SLLPushOrInit(TokenEle, T->TokensRoot, T->TokensHead);
}

internal bool
TokenizerAtEnd(gs_tokenizer* T)
{
  return (T->At >= T->Len);
}

internal void
TokenizerAdvanceChar(gs_tokenizer* T)
{
  if (TokenizerAtEnd(T)) return;
  if (IsNewline(*T->CharAt)) {
    T->Row++;
    T->Col = 0;
  }
  T->At++;
  T->Col++;
  T->CharAt = T->Base + T->At;
}

internal gs_char_type
TokenizerAdvanceToTokenBoundary(gs_tokenizer* T)
{
  gs_char_type Type = GetCharType(*T->CharAt);
  if (Type == CharType_Alpha)
  {
    Type = CharType_AlphaNum;
    while (!TokenizerAtEnd(T))
    {
      gs_char_type NewType =  GetCharType(*T->CharAt);
      if (NewType != CharType_Alpha && NewType != CharType_Numeric) break;
      TokenizerAdvanceChar(T);
    }
  }
  else if (Type == CharType_Symbol && !TokenizerAtEnd(T))
  {
    TokenizerAdvanceChar(T);
  }
  else
  {
    while (!TokenizerAtEnd(T) && GetCharType(*T->CharAt) == Type)
    {
      TokenizerAdvanceChar(T);
    }
  }
  return Type;
}

internal void
TokenizerAdvanceToken(gs_tokenizer* T)
{
  // NOTE(PS): eat whitespace and newlines
  while (!TokenizerAtEnd(T) && 
         (IsNewline(*T->CharAt) || IsWhitespace(*T->CharAt))) 
  {
    TokenizerAdvanceChar(T);
  }
  
  char* TokenStart = T->CharAt;
  gs_char_type TokenType = TokenizerAdvanceToTokenBoundary(T);
  char* TokenEnd = T->CharAt;
  
  TokenizerPushToken(T, TokenStart, TokenEnd, TokenType);
}

internal void
TokenizerTokenizeAll(gs_tokenizer* T)
{
  while (!TokenizerAtEnd(T)) 
  {
    TokenizerAdvanceToken(T);
  }
}

//
// Tokens

internal bool
TokenEqualsCStr(gs_token T, char* Str, u64 StrLen)
{
  if (T.Len != StrLen) return false;
  for (u64 i = 0; i < StrLen; i++)
  {
    if (T.Data[i] != Str[i]) return false;
  }
  return true;
}

internal bool
TokenEquals(gs_token T, char* Str)
{
  u64 StrLen = CStringLength(Str);
  return TokenEqualsCStr(T, Str, StrLen);
}

#endif //GS_TOKENIZER_H
