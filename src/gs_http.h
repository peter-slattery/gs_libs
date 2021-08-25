/* date = August 25th 2021 1:00 am */

#ifndef GS_HTTP_H
#define GS_HTTP_H

typedef enum
{
  HTTPMethod_Invalid,
  
  HTTPMethod_GET,
  HTTPMethod_HEAD,
  HTTPMethod_POST,
  HTTPMethod_PUT,
  HTTPMethod_DELETE,
  HTTPMethod_CONNECT,
  HTTPMethod_OPTIONS,
  HTTPMethod_TRACE,
  
  HTTPMethod_Count,
} http_method;

global gs_string HTTPMethodStrings[] =  {
  LitStr("HTTPMethod_Invalid"),
  
  LitStr("GET"),
  LitStr("HEAD"),
  LitStr("POST"),
  LitStr("PUT"),
  LitStr("DELETE"),
  LitStr("CONNECT"),
  LitStr("OPTIONS"),
  LitStr("TRACE"),
  
  LitStr("HTTPMethod_Count"),
};

global u64 HTTPMethodHashes[HTTPMethod_Count];

typedef enum
{
  HTTPField_Invalid,
  
  HTTPField_Accept,
  HTTPField_AcceptCharset,
  HTTPField_AcceptEncoding,
  HTTPField_AcceptLanguage,
  HTTPField_Authorization,
  HTTPField_CacheControl,
  HTTPField_Connection,
  HTTPField_Expect,
  HTTPField_From,
  HTTPField_Host,
  HTTPField_IfMatch,
  HTTPField_IfModifiedSince,
  HTTPField_IfNoneMatch,
  HTTPField_IfRange,
  HTTPField_IfUnmodifiedSince,
  HTTPField_MaxForwards,
  HTTPField_Origin,
  HTTPField_Pragma,
  HTTPField_ProxyAuthorization,
  HTTPField_Range,
  HTTPField_Referer,
  HTTPField_SecGPC,
  HTTPField_SecFetchSite,
  HTTPField_SecFechMode,
  HTTPField_SecFetchUser,
  HTTPField_SecFetchDest,
  HTTPField_SecWebSocketVersion,
  HTTPField_SecWebSocketKey,
  HTTPField_SecWebSocketExtensions,
  HTTPField_TE,
  HTTPField_Upgrde,
  HTTPField_UpgradeInsecureRequests, 
  HTTPField_UserAgent,
  
  HTTPField_Count
} http_field_type;

global gs_string HTTPFieldStrings[] = {
  LitStr("HTTPField_Invalid"),
  
  LitStr("Accept"),
  LitStr("Accept-Charset"),
  LitStr("Accept-Encoding"),
  LitStr("Accept-Language"),
  LitStr("Authorization"),
  LitStr("Cache-Control"),
  LitStr("Connection"),
  LitStr("Expect"),
  LitStr("From"),
  LitStr("Host"),
  LitStr("IfMatch"),
  LitStr("If-Modified-Since"),
  LitStr("If-None-Match"),
  LitStr("If-Range"),
  LitStr("If-Unmodified-Since"),
  LitStr("Max-Forwards"),
  LitStr("Origin"),
  LitStr("Pragma"),
  LitStr("Proxy-Authorization"),
  LitStr("Range"),
  LitStr("Referer"),
  LitStr("Sec-GPC"),
  LitStr("Sec-Fetch-Site"),
  LitStr("Sec-Fetch-Mode"),
  LitStr("Sec-Fetch-User"),
  LitStr("Sec-Fetch-Dest"),
  LitStr("Sec-WebSocket-Version"),
  LitStr("Sec-WebSocket-Key"),
  LitStr("Sec-WebSocket-Extensions"),
  LitStr("TE"),
  LitStr("Upgrade"),
  LitStr("Upgrade-Insecure-Requests"),
  LitStr("User-Agent"),
  
  LitStr("HTTPField_Count"),
};

global u64 HTTPFieldHashes[HTTPField_Count];

global char* WebSocketAcceptMagicStr = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

typedef struct
{
  gs_string AbsolutePath;
  gs_string Query;
} http_uri;

typedef struct
{
  http_field_type Type;
  gs_string Value;
} http_field;

typedef struct
{
  // Request Line
  http_method Method;
  http_uri URI;
  
  s32 FieldsCap;
  s32 FieldsLen;
  http_field* Fields;
  
  gs_string Body;
} http_request;

internal u64
HTTPHash(gs_string S)
{
  // Source: DJB2 Hash
  u64 Hash = 5381;
  for (u32 i = 0; i < S.Len; i++)
  {
    Hash = ((Hash << 5) + Hash) + S.Data[i];
  }
  return Hash;
}

internal void
HTTPInit()
{
  for (u64 i = 0; i < HTTPMethod_Count; i++)
  {
    HTTPMethodHashes[i] = HTTPHash(HTTPMethodStrings[i]);
  }
  
  for (u64 i = 0; i < HTTPField_Count; i++)
  {
    HTTPFieldHashes[i] = HTTPHash(HTTPFieldStrings[i]);
  }
}

internal u64
HTTPLookupHashIndex(u64 Hash, u64* HashArr, u64 HashArrLen)
{
  for (u64 i = 0; i < HashArrLen; i++)
  {
    if (Hash == HashArr[i])
    {
      return i;
    }
  }
  // return 0 by default since all cases we use this for
  // have a *_Invalid as index 0
  return 0;
}

internal http_method
HTTPParseMethod (gs_string Method)
{
  http_method Result = HTTPMethod_Invalid;
  u64 MHash = HTTPHash(Method);
  Result = (http_method)HTTPLookupHashIndex(MHash, 
                                            HTTPMethodHashes, 
                                            HTTPMethod_Count);
  return Result;
}

internal http_field_type
HTTPParseFieldType (gs_string Field)
{
  http_field_type Result = HTTPField_Invalid;
  u64 FHash = HTTPHash(Field);
  Result = (http_field_type)HTTPLookupHashIndex(FHash,
                                                HTTPFieldHashes,
                                                HTTPField_Count);
  return Result;
}

internal http_field
HTTPParseField(gs_token_iter* TI)
{
  http_field Result = {};
  
  gs_token StartFieldTok = TokenIterAdvance(TI);
  while (!TokenIterPeek(TI, ":")) { TokenIterAdvance(TI); }
  gs_string FieldStr = TokenConcat(StartFieldTok, TI->At);
  Result.Type = HTTPParseFieldType(FieldStr);
  
  if (Result.Type == HTTPField_Invalid ||
      !TokenIterMatchNext(TI, ":"))
  {
    printf("Error: misformed HTTP Field. %.*s\n",
           (u32)FieldStr.Len,
           (char*)FieldStr.Data);
  } 
  
  gs_token StartValueTok = TokenIterAdvance(TI);
  while (TI->Next.Row == StartFieldTok.Row) { TokenIterAdvance(TI); }
  Result.Value = TokenConcat(StartValueTok, TI->At);
  
  return Result;
}

internal http_request
HTTPParseRequest(gs_data Req, gs_memory_arena* Arena)
{
  http_request Result = {};
  
  gs_tokenizer T = TokenizerCreate((char*)Req.Memory, Req.Size, Arena);
  gs_token_iter TI = TokenizerTokenizeAll(&T);
  
  // Request Line
  gs_token MethodToken = TokenIterAdvance(&TI);
  http_method Method = HTTPParseMethod(MethodToken.Str);
  if (Method == HTTPMethod_Invalid) 
  {
    printf("Error: Received invalid method: %.*s\n",
           (u32)MethodToken.Str.Len,
           (char*)MethodToken.Str.Data);
    return Result;
  }
  
  // Resource
  gs_token ResourceToken0 = TokenIterAdvance(&TI);
  while (!TokenIterPeek(&TI, "HTTP")) TokenIterAdvance(&TI);
  gs_string ResourceStr = TokenConcat(ResourceToken0, TI.At);
  printf("Resource: %.*s\n", (u32)ResourceStr.Len, (char*)ResourceStr.Data);
  
  // we don't care about the rest of the row for now
  // this is the HTTP/1.1 or HTTP/1.2 section
  while (TI.Next.Row == ResourceToken0.Row) { TokenIterAdvance(&TI); }
  
  // HTTP Fields
  // http fields are one per line, and the list ends when
  // there are no more tokens or there is a blank line
  // so we can look ahead to count the correct number of
  // fields to allocate
  Result.FieldsCap = 0;
  for (gs_token_sll* At = TI.SLLAt; 
       At != 0 && At->Next->T.Row < At->T.Row + 2;
       At = At->Next)
  {
    Result.FieldsCap++;
  }
  Result.Fields = PushArray(Arena, http_field, Result.FieldsCap);
  
  while (TokenIterCanAdvance(&TI) && TI.Next.Row < TI.At.Row + 2)
  {
    Assert(Result.FieldsLen < Result.FieldsCap);
    http_field F = HTTPParseField(&TI);
    printf("Field: %s: %.*s\n",
           (char*)HTTPFieldStrings[F.Type].Data,
           (u32)F.Value.Len,
           (char*)F.Value.Data);
    
    http_field* D = Result.Fields + Result.FieldsLen++;
    D->Type = F.Type;
    D->Value = PushString(Arena, F.Value.Len);
    StringCopy(F.Value, &D->Value);
  }
  
  // the body of the message
  while (TokenIterCanAdvance(&TI))
  {
    gs_token Tok = TokenIterAdvance(&TI);
    printf("Tok: %lld:%lld - %.*s\n",
           Tok.Row, 
           Tok.Col,
           (u32)Tok.Str.Len,
           (char*)Tok.Str.Data);
  }
  
  return Result;
}

internal http_field
HTTPRequestFindField(http_request Req, http_field_type F)
{
  for (u64 i = 0; i < Req.FieldsLen; i++)
  {
    if (Req.Fields[i].Type == F) return Req.Fields[i];
  }
  return (http_field){0};
}

#if 0
internal data_buffer
BuildHTTPResponse(http_request Request, char* ResponseBody, s32 ResponseBodySize, gs_memory_arena* Memory)
{
  data_buffer Result = {};
  
  s32 HeaderContentLength = ResponseBodySize;
  //if (ResponseBody[ResponseBodySize - 1] == 0) { HeaderContentLength -= 1; }
  
  Result.Size = ResponseBodySize + 2048;
  Result.Buffer = PushArray(Memory, char, Result.Size);
  Result.Used = snprintf(Result.Buffer, Result.Size, "HTTP/1.1 200 OK\nConnection: close\nContent-Length: %d\n",
                         HeaderContentLength);
  
  for (s32 FieldIdx = 0; FieldIdx < Request.FieldsCount; FieldIdx++)
  {
    http_field* Field = Request.Fields + FieldIdx;
    
    switch (Field->Type)
    {
      case HTTPField_Accept:
      {
        s32 IndexOfFirstComma = IndexOfChar(Field->Value, ',');
        s32 IndexOfSemiColon = IndexOfChar(Field->Value, ';');
        s32 EndOfType = GSMin(IndexOfFirstComma, IndexOfSemiColon);
        // NOTE(Peter): If its less than zero, then one of the fields wasn't found.
        if (EndOfType <= 0) { EndOfType = GSMax(IndexOfSemiColon, IndexOfFirstComma); }
        
        if (EndOfType > 0)
        {
          gs_string Value = Substring(Field->Value, 0, EndOfType);
          
          Result.Used += snprintf(Result.Buffer + Result.Used, Result.Size - Result.Used,
                                  "Content-Type:%.*s\n",
                                  Value.Length, Value.Memory);
        }
      }break;
    }
  }
  
  Result.Used += snprintf(Result.Buffer + Result.Used, Result.Size - Result.Used, "\n\n%s", ResponseBody);
  
  return Result;
}
#endif


#endif //GS_HTTP_H
