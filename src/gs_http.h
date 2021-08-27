/* date = August 25th 2021 1:00 am */

#ifndef GS_HTTP_H
#define GS_HTTP_H

//
// Requests
//

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
  HTTPField_AcceptRanges,
  HTTPField_Authorization,
  HTTPField_CacheControl,
  HTTPField_Connection,
  HTTPField_ContentLength,
  HTTPField_ContentType,
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
  HTTPField_SecWebSocketAccept,
  HTTPField_SecWebSocketVersion,
  HTTPField_SecWebSocketKey,
  HTTPField_SecWebSocketExtensions,
  HTTPField_Server,
  HTTPField_TE,
  HTTPField_TransferEncoding,
  HTTPField_Upgrade,
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
  LitStr("Accept-Ranges"),
  LitStr("Authorization"),
  LitStr("Cache-Control"),
  LitStr("Connection"),
  LitStr("Content-Length"),
  LitStr("Content-Type"),
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
  LitStr("Sec-WebSocket-Accept"),
  LitStr("Sec-WebSocket-Version"),
  LitStr("Sec-WebSocket-Key"),
  LitStr("Sec-WebSocket-Extensions"),
  LitStr("Server"),
  LitStr("TE"),
  LitStr("Transfer-Encoding"),
  LitStr("Upgrade"),
  LitStr("Upgrade-Insecure-Requests"),
  LitStr("User-Agent"),
  
  LitStr("HTTPField_Count"),
};

global u64 HTTPFieldHashes[HTTPField_Count];

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

//
// Responses
//

typedef enum
{
  HTTPVersion_Invalid,
  
  HTTPVersion_11,
  HTTPVersion_12,
} http_version;

global gs_string HTTPVersionStrings[] = {
  LitStr("HTTPVersion_Invalid"),
  LitStr("HTTP/1.1"),
  LitStr("HTTP/1.2"),
};

typedef enum
{
  // Information
  HTTPResponse_Continue,
  HTTPResponse_SwitchingProtocol,
  HTTPResponse_Processing,
  HTTPResponse_EarlyHints,
  
  // Sucess
  HTTPResponse_OK,
  HTTPResponse_Created,
  HTTPResponse_Accepted,
  HTTPResponse_NonAuthoritativeInformation,
  HTTPResponse_NoContent,
  HTTPResponse_ResetContent,
  HTTPResponse_PartialContent,
  HTTPResponse_MultiStatus,
  HTTPResponse_AlreadyReported,
  HTTPResponse_IMused,
  
  // Redirection 
  HTTPResponse_MultipleChoice,
  HTTPResponse_MovedPermanently,
  HTTPResponse_Found,
  HTTPResponse_SeeOther,
  HTTPResponse_NotModified,
  HTTPResponse_TemporaryRedirect,
  HTTPResponse_PermanentRedirect,
  
  // Client Error Responses
  HTTPResponse_BadRequest,
  HTTPResponse_Unauthorized,
  HTTPResponse_PaymentRequired,
  HTTPResponse_Forbidden,
  HTTPResponse_NotFound,
  HTTPResponse_MethodNotAllowed,
  HTTPResponse_NotAcceptable,
  HTTPResponse_ProxyAuthenticationRequired,
  HTTPResponse_RequestTimeout,
  HTTPResponse_Conflict,
  HTTPResponse_Gone,
  HTTPResponse_LengthRequired,
  HTTPResponse_PreconditionFailed,
  HTTPResponse_PayloadTooLarge,
  HTTPResponse_URITooLong,
  HTTPResponse_UnsupportedMediaType,
  HTTPResponse_RangeNotSatisfiable,
  HTTPResponse_ExpectationFailed,
  HTTPResponse_ImATeapot,
  HTTPResponse_MisdirectedRequest,
  HTTPResponse_UnprocessableEntity,
  HTTPResponse_Locked,
  HTTPResponse_FailedDependency,
  HTTPResponse_TooEarly,
  HTTPResponse_UpgradeRequired,
  HTTPResponse_PreconditionRequired,
  HTTPResponse_TooManyRequests,
  HTTPResponse_RequestHeaderFieldsTooLarge,
  HTTPResponse_UnavailableForLegalReasons,
  
  // Server Error Responses
  HTTPResponse_InternalServerError,
  HTTPResponse_NotImplemented,
  HTTPResponse_BadGateway,
  HTTPResponse_ServiceUnavilable,
  HTTPResponse_GatewayTimeout,
  HTTPResponse_HTTPVersionNotSupported,
  HTTPResponse_VariantAlsoNegotiates,
  HTTPResponse_InsufficientStorage,
  HTTPResponse_LoopDetected,
  HTTPResponse_NotExtended,
  HTTPResponse_NetworkAuthenticationRequired,
} http_response_status;

global u32 HTTPResponseStatusNumber[] = {
  /* HTTPResponse_Continue */ 100,
  /* HTTPResponse_SwitchingProtocol */ 101,
  /* HTTPResponse_Processing */ 102,
  /* HTTPResponse_EarlyHints */ 103,
  
  // Sucess
  /* HTTPResponse_OK */ 200,
  /* HTTPResponse_Created */ 201,
  /* HTTPResponse_Accepted */ 202,
  /* HTTPResponse_NonAuthoritativeInformation */ 203,
  /* HTTPResponse_NoContent */ 204,
  /* HTTPResponse_ResetContent */ 205,
  /* HTTPResponse_PartialContent */ 206,
  /* HTTPResponse_MultiStatus */ 207,
  /* HTTPResponse_AlreadyReported */ 208,
  /* HTTPResponse_IMused */ 226,
  
  // Redirection 
  /* HTTPResponse_MultipleChoice */ 300,
  /* HTTPResponse_MovedPermanently */ 301,
  /* HTTPResponse_Found */ 302,
  /* HTTPResponse_SeeOther */ 303,
  /* HTTPResponse_NotModified */ 304,
  /* HTTPResponse_TemporaryRedirect */ 307,
  /* HTTPResponse_PermanentRedirect */ 308,
  
  // Client Error Responses
  /* HTTPResponse_BadRequest */ 400,
  /* HTTPResponse_Unauthorized */ 401,
  /* HTTPResponse_PaymentRequired */ 402,
  /* HTTPResponse_Forbidden */ 403,
  /* HTTPResponse_NotFound */ 404,
  /* HTTPResponse_MethodNotAllowed */ 405,
  /* HTTPResponse_NotAcceptable */ 406,
  /* HTTPResponse_ProxyAuthenticationRequired */ 407,
  /* HTTPResponse_RequestTimeout */ 408,
  /* HTTPResponse_Conflict */ 409,
  /* HTTPResponse_Gone */ 410,
  /* HTTPResponse_LengthRequired */ 411,
  /* HTTPResponse_PreconditionFailed */ 412,
  /* HTTPResponse_PayloadTooLarge */ 413,
  /* HTTPResponse_URITooLong */ 414,
  /* HTTPResponse_UnsupportedMediaType */ 415,
  /* HTTPResponse_RangeNotSatisfiable */ 416,
  /* HTTPResponse_ExpectationFailed */ 417,
  /* HTTPResponse_ImATeapot */ 418,
  /* HTTPResponse_MisdirectedRequest */ 421,
  /* HTTPResponse_UnprocessableEntity */ 422,
  /* HTTPResponse_Locked */ 423,
  /* HTTPResponse_FailedDependency */ 424,
  /* HTTPResponse_TooEarly */ 425,
  /* HTTPResponse_UpgradeRequired */ 426,
  /* HTTPResponse_PreconditionRequired */ 428,
  /* HTTPResponse_TooManyRequests */ 429,
  /* HTTPResponse_RequestHeaderFieldsTooLarge */ 431,
  /* HTTPResponse_UnavailableForLegalReasons */ 451,
  
  // Server Error Responses
  /* HTTPResponse_InternalServerError */ 500,
  /* HTTPResponse_NotImplemented */ 501,
  /* HTTPResponse_BadGateway */ 502,
  /* HTTPResponse_ServiceUnavilable */ 503,
  /* HTTPResponse_GatewayTimeout */ 504,
  /* HTTPResponse_HTTPVersionNotSupported */ 505,
  /* HTTPResponse_VariantAlsoNegotiates */ 506,
  /* HTTPResponse_InsufficientStorage */ 507,
  /* HTTPResponse_LoopDetected */ 508,
  /* HTTPResponse_NotExtended */ 510,
  /* HTTPResponse_NetworkAuthenticationRequired */ 511,
  
};

global gs_string HTTPResponseStatusStrings[] = {
  // Information
  LitStr("Continue"),
  LitStr("Switching Protocol"),
  LitStr("Processing"),
  LitStr("Early Hints"),
  
  // Sucess
  LitStr("OK"),
  LitStr("Created"),
  LitStr("Accepted"),
  LitStr("Non-Authoritative Information"),
  LitStr("No Content"),
  LitStr("Reset Content"),
  LitStr("Partial Content"),
  LitStr("Multi-Status"),
  LitStr("Already Reported"),
  LitStr("IM Used"),
  
  // Redirection 
  LitStr("Multiple Choice"),
  LitStr("Moved Permanently"),
  LitStr("Found"),
  LitStr("See Other"),
  LitStr("Not Modified"),
  LitStr("Temporary Redirect"),
  LitStr("Permanent Redirect"),
  
  // Client Error Responses
  LitStr("BadRequest"),
  LitStr("Unauthorized"),
  LitStr("Payment Required"),
  LitStr("Forbidden"),
  LitStr("Not Found"),
  LitStr("Method Not Allowed"),
  LitStr("Not Acceptable"),
  LitStr("Proxy Authentication Required"),
  LitStr("Request Timeout"),
  LitStr("Conflict"),
  LitStr("Gone"),
  LitStr("Length Required"),
  LitStr("Precondition Failed"),
  LitStr("Payload Too Large"),
  LitStr("URI Too Long"),
  LitStr("Unsupported Media Type"),
  LitStr("Range Not Satisfiable"),
  LitStr("Expectation Failed"),
  LitStr("I'm A Teapot"),
  LitStr("Misdirected Request"),
  LitStr("Unprocessable Entity"),
  LitStr("Locked"),
  LitStr("Failed Dependency"),
  LitStr("Too Early"),
  LitStr("Upgrade Required"),
  LitStr("Precondition Required"),
  LitStr("Too Many Requests"),
  LitStr("Request Header Fields Too Large"),
  LitStr("Unavailable For Legal Reasons"),
  
  // Server Error Responses
  LitStr("Internal Server Error"),
  LitStr("Not Implemented"),
  LitStr("Bad Gateway"),
  LitStr("Service Unavilable"),
  LitStr("Gateway Timeout"),
  LitStr("HTTP Version Not Supported"),
  LitStr("Variant Also Negotiates"),
  LitStr("Insufficient Storage"),
  LitStr("Loop Detected"),
  LitStr("Not Extended"),
  LitStr("Network Authentication Required"),
  
};

typedef struct
{
  http_version Version;
  http_response_status Status;
  
  u64 FieldsCap;
  u64 FieldsLen;
  http_field* Fields;
  
  gs_string Body;
} http_response;

//
// Mime Types
//

typedef enum 
{
  MimeType_Invalid,
  
  // Discrete types
  MimeType_Application,
  MimeType_ApplicationOctetStream,
  MimeType_ApplicationWasm,
  
  MimeType_Audio,
  MimeType_AudioWav,
  MimeType_AudioWebm,
  MimeType_AudioOgg,
  
  MimeType_Example,
  MimeType_Font,
  
  MimeType_Image,
  MimeType_ImageAPNG,
  MimeType_ImageAVIF,
  MimeType_ImageGIF,
  MimeType_ImageJPEG,
  MimeType_ImagePNG,
  MimeType_ImageSVG,
  MimeType_ImageWebP,
  
  MimeType_Model,
  
  MimeType_Text,
  MimeType_TextPlain,
  MimeType_TextCss,
  MimeType_TextHtml,
  MimeType_TextJavascript,
  
  MimeType_Video,
  MimeType_VideoWebm,
  MimeType_VideoOgg,
  MimeType_VideoMp4,
  
  // Multipart Types
  MimeType_Message,
  
  MimeType_Multipart,
  MimeType_MultipartFormData,
  MimeType_MultipartByteRanges,
  
  MimeType_Count,
} mime_type;

global gs_string MimeTypeStrings[] = {
  LitStr("MimeType_Invalid"),
  
  LitStr("application"),
  LitStr("application/octet-stream"),
  LitStr("application/wasm"),
  
  LitStr("audio"),
  LitStr("audio/audio-wav"),
  LitStr("audio/audio-webm"),
  LitStr("audio/audio-ogg"),
  
  LitStr("example"),
  LitStr("font"),
  
  LitStr("image"),
  LitStr("image/apng"),
  LitStr("image/avif"),
  LitStr("image/gif"),
  LitStr("image/jpeg"),
  LitStr("image/png"),
  LitStr("image/svg"),
  LitStr("image/webp"),
  
  LitStr("model"),
  
  LitStr("text"),
  LitStr("text/plain"),
  LitStr("text/css"),
  LitStr("text/html"),
  LitStr("text/javascript"),
  
  LitStr("video"),
  LitStr("video/webm"),
  LitStr("video/ogg"),
  LitStr("video/mp4"),
  
  LitStr("message"),
  
  LitStr("multipart"),
  LitStr("multipart/form-data"),
  LitStr("multipart/byte-ranges"),
  
  LitStr("MimeType_Count"),
};

#define MIME_HASH_TABLE_CAP 2048
global u64 FileExtHashToMimeTypeKeys[MIME_HASH_TABLE_CAP];
global mime_type FileExtHashToMimeTypeValues[MIME_HASH_TABLE_CAP];

// TODO(PS): make this a hashtable?
global gs_string MimeTypeToFileExt[] = {
  
};

//
// Utilities
//

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

//
// Mime Types
//

internal void
MimeRegisterFileType(gs_string Ext, mime_type Type)
{
  u64 H = HTTPHash(Ext);
  u64 Index = H % MIME_HASH_TABLE_CAP;
  u64 StartIndex = Index;
  do {
    if (FileExtHashToMimeTypeKeys[Index] == 0) break;
    Index++;
    if (Index >= MIME_HASH_TABLE_CAP) Index = 0;
  } while (Index != StartIndex);
  Assert(FileExtHashToMimeTypeKeys[Index] == 0);
  
  FileExtHashToMimeTypeKeys[Index] = H;
  FileExtHashToMimeTypeValues[Index] = Type;
}

internal mime_type
MimeTypeGetFromFileExt(gs_string Ext)
{
  u64 H = HTTPHash(Ext);
  u64 Index = H % MIME_HASH_TABLE_CAP;
  u64 StartIndex = Index;
  do {
    u64 V = FileExtHashToMimeTypeKeys[Index];
    if (V == 0 || V == H) break;
    Index++;
    if (Index >= MIME_HASH_TABLE_CAP) Index = 0;
  } while (Index != StartIndex);
  
  if (FileExtHashToMimeTypeKeys[Index] == 0) return MimeType_Invalid;
  return FileExtHashToMimeTypeValues[Index];
}
internal gs_string
MimeTypeGetStrFromFileExt(gs_string Ext)
{
  mime_type T = MimeTypeGetFromFileExt(Ext);
  if (T == MimeType_Invalid) return (gs_string){};
  return MimeTypeStrings[T];
}

//
// Lib
//

internal void
HTTPInit()
{
  // Method Hashes
  for (u64 i = 0; i < HTTPMethod_Count; i++)
  {
    HTTPMethodHashes[i] = HTTPHash(HTTPMethodStrings[i]);
  }
  for (u64 i = 0; i < HTTPField_Count; i++)
  {
    HTTPFieldHashes[i] = HTTPHash(HTTPFieldStrings[i]);
  }
  
  // Mime Type Registration
  MemoryClearArray(FileExtHashToMimeTypeKeys, u64, MIME_HASH_TABLE_CAP);
  MemoryClearArray(FileExtHashToMimeTypeValues, mime_type, MIME_HASH_TABLE_CAP);
  MimeRegisterFileType(LitStr(".wasm"), MimeType_ApplicationWasm);
  MimeRegisterFileType(LitStr(".wav"), MimeType_AudioWav);
  MimeRegisterFileType(LitStr(".webm"), MimeType_AudioWebm);
  MimeRegisterFileType(LitStr(".ogg"), MimeType_AudioOgg);
  MimeRegisterFileType(LitStr(".ttf"), MimeType_Font);
  MimeRegisterFileType(LitStr(".otf"), MimeType_Font);
  MimeRegisterFileType(LitStr(".apng"), MimeType_ImageAPNG);
  MimeRegisterFileType(LitStr(".avif"), MimeType_ImageAVIF);
  MimeRegisterFileType(LitStr(".gif"), MimeType_ImageGIF);
  MimeRegisterFileType(LitStr(".jpeg"), MimeType_ImageJPEG);
  MimeRegisterFileType(LitStr(".jpg"), MimeType_ImageJPEG);
  MimeRegisterFileType(LitStr(".png"), MimeType_ImagePNG);
  MimeRegisterFileType(LitStr(".svg"), MimeType_ImageSVG);
  MimeRegisterFileType(LitStr(".webp"), MimeType_ImageWebP);
  MimeRegisterFileType(LitStr(".txt"), MimeType_TextPlain);
  MimeRegisterFileType(LitStr(".css"), MimeType_TextCss);
  MimeRegisterFileType(LitStr(".html"), MimeType_TextHtml);
  MimeRegisterFileType(LitStr(".js"), MimeType_TextJavascript);
  MimeRegisterFileType(LitStr(".webm"), MimeType_VideoWebm);
  MimeRegisterFileType(LitStr(".ogg"), MimeType_VideoOgg);
  MimeRegisterFileType(LitStr(".mp4"), MimeType_VideoMp4);
}

//
// Requests
//

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
  Result.Method = HTTPParseMethod(MethodToken.Str);
  if (Result.Method == HTTPMethod_Invalid) return Result;
  
  // Resource
  gs_token ResourceToken0 = TokenIterAdvance(&TI);
  while (!TokenIterPeek(&TI, "?") &&
         !TokenIterPeek(&TI, "HTTP")) TokenIterAdvance(&TI);
  Result.URI.AbsolutePath = TokenConcatCopy(ResourceToken0, TI.At, Arena);
  
  if (TokenIterMatchNext(&TI, "?"))
  {
    gs_token QueryToken0 = TokenIterAdvance(&TI);
    while (!TokenIterPeek(&TI, "HTTP")) TokenIterAdvance(&TI);
    Result.URI.Query = TokenConcat(QueryToken0, TI.At);
  }
  
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
    
    http_field* D = Result.Fields + Result.FieldsLen++;
    D->Type = F.Type;
    D->Value = PushString(Arena, F.Value.Len);
    StringCopy(F.Value, &D->Value);
  }
  
  // the body of the message
  if (TokenIterCanAdvance(&TI))
  {
    gs_token BodyToken0 = TokenIterAdvance(&TI);
    while (TokenIterCanAdvance(&TI)) TokenIterAdvance(&TI);
    Result.Body = TokenConcat(BodyToken0, TI.At);
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

internal bool
HTTPRequestHasField(http_request Req, http_field_type F)
{
  http_field Field = HTTPRequestFindField(Req, F);
  return(Field.Type != HTTPField_Invalid);
}

//
// Responses
//

internal http_response
HTTPResPush(gs_memory_arena* Arena, u64 FieldsCap)
{
  http_response Result = {};
  Result.FieldsCap = FieldsCap;
  Result.Fields = PushArray(Arena, http_field, FieldsCap);
  return Result;
}

internal void
HTTPResPushField(http_response* Res, http_field Field)
{
  Res->Fields[Res->FieldsLen++] = Field;
}

internal http_response
HTTPResBuildFile(gs_string File, gs_string ContentType, gs_memory_arena* Arena)
{
  http_response Res = HTTPResPush(Arena, 6);
  Res.Version = HTTPVersion_11;
  Res.Status = HTTPResponse_OK;
  HTTPResPushField(&Res,
                   (http_field) {
                     .Type = HTTPField_AcceptRanges,
                     .Value = LitStr("bytes"),
                   });
  HTTPResPushField(&Res,
                   (http_field) {
                     .Type = HTTPField_CacheControl,
                     .Value = LitStr("no-cache"),
                   });
  HTTPResPushField(&Res,
                   (http_field) {
                     .Type = HTTPField_Connection,
                     .Value = LitStr("keep-alive"),
                   });
  HTTPResPushField(&Res,
                   (http_field) {
                     .Type = HTTPField_Server,
                     .Value = LitStr("mole"),
                   });
  HTTPResPushField(&Res, 
                   (http_field) {
                     .Type = HTTPField_ContentType,
                     .Value = ContentType,
                   });
  // U64 Max is ~20 digits long
  gs_string LenStr = PushString(Arena, 32);
  ToStringU64(File.Len, DigitsDecimal, &LenStr);
  HTTPResPushField(&Res, 
                   (http_field) {
                     .Type = HTTPField_ContentLength,
                     .Value = LenStr,
                   });
  
  Res.Body = File;
  return Res;
}

internal http_response
HTTPResBuildFileNotFound()
{
  http_response Res = {};
  Res.Version = HTTPVersion_11;
  Res.Status = HTTPResponse_NotFound;
  return Res;
}

internal gs_string
HTTPResToString(http_response Res, gs_memory_arena* Arena)
{
  // Collect Elements
  gs_string HTTPVersion = HTTPVersionStrings[Res.Version];
  gs_string HTTPStatus = HTTPResponseStatusStrings[Res.Status];
  gs_string Body = Res.Body;
  
  u64 NewLineSize = 2; // "/r/n"
  u64 FieldSeparatorSize = 2; // ": "
  u64 Cap = (HTTPVersion.Len + NewLineSize + 
             3 + NewLineSize + // this is for the status code
             HTTPStatus.Len + NewLineSize +
             Body.Len + NewLineSize + NewLineSize);
  for (u64 i = 0; i < Res.FieldsLen; i++)
  {
    http_field F = Res.Fields[i];
    gs_string FStr = HTTPFieldStrings[F.Type];
    Cap += FStr.Len + FieldSeparatorSize + F.Value.Len + NewLineSize;
  }
  
  gs_string Result = PushString(Arena, Cap);
  
  // Status Line
  Result.Len += snprintf((char*)Result.Data, (u32)(Result.Cap - Result.Len),
                         "%.*s %d %.*s\r\n",
                         StrExpand(HTTPVersion),
                         HTTPResponseStatusNumber[Res.Status],
                         StrExpand(HTTPStatus));
  // Fields
  for (u64 i = 0; i < Res.FieldsLen; i++)
  {
    http_field F = Res.Fields[i];
    gs_string FStr = HTTPFieldStrings[F.Type];
    Result.Len += snprintf((char*)Result.Data + Result.Len, 
                           (u32)(Result.Cap - Result.Len),
                           "%.*s: %.*s\r\n",
                           StrExpand(FStr),
                           StrExpand(F.Value));
  }
  Result.Len += snprintf((char*)Result.Data + Result.Len, 
                         (u32)(Result.Cap - Result.Len),
                         "\r\n");
  
  Assert(Result.Len + Res.Body.Len <= Result.Cap);
  for (u64 i = 0; i < Res.Body.Len; i++)
  {
    Result.Data[Result.Len + i] = Res.Body.Data[i];
  }
  Result.Len += Res.Body.Len;
  
  
  return Result;
}

//
// Websockets
//

global gs_string HTTPWebSocketHandshakeMagicStr = LitStr("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

// Creates the handshake string that should
// be sent back to the client in order to 
// initiate a websocket connection.
//
// The return header should contain the following
// field:
//
//    Sec-WebSocket-Accept: <handshake_str>
//
// where <handshake_str> is the result of this 
// method.
//
// Source: https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API/Writing_WebSocket_servers#server_handshake_response
internal gs_string
HTTPWebSocketHandshakeCreate(http_request Req, gs_memory_arena* Arena)
{
  gs_string Result = {};
  
  http_field WSKey = HTTPRequestFindField(Req, HTTPField_SecWebSocketKey);
  if (WSKey.Type == HTTPField_Invalid) return Result; // Error
  
  u64 KeyLen = HTTPWebSocketHandshakeMagicStr.Len;
  KeyLen += WSKey.Value.Len;
  gs_string MagicKey = PushString(Arena, KeyLen);
  StringCopy(WSKey.Value, &MagicKey);
  StringAppendString(&MagicKey, HTTPWebSocketHandshakeMagicStr);
  
  gs_sha1_ctx SHA;
  SHA1Reset(&SHA);
  SHA1Input(&SHA, MagicKey.Data, MagicKey.Len);
  if (!SHA1Result(&SHA)) return Result; // Error
  
  u32 SHAHashResEndianSwapped[5] = {
    EndianSwapU32(SHA.MsgDigest[0]),
    EndianSwapU32(SHA.MsgDigest[1]),
    EndianSwapU32(SHA.MsgDigest[2]),
    EndianSwapU32(SHA.MsgDigest[3]),
    EndianSwapU32(SHA.MsgDigest[4]),
  };
  
  Result.Data = Encode_Base64Alloc((u8*)SHAHashResEndianSwapped,
                                   sizeof(SHAHashResEndianSwapped),
                                   &Result.Len,
                                   Arena);
  
  return Result;
}

internal http_response
HTTPWebSocketHandshakeResponse(http_request Req, gs_memory_arena* Arena)
{
  http_response Res = HTTPResPush(Arena, 3);
  Res.Version = HTTPVersion_11;
  Res.Status = HTTPResponse_SwitchingProtocol;
  Res.Fields[0] = (http_field) {
    .Type = HTTPField_Upgrade,
    .Value = LitStr("websocket"),
  };
  Res.Fields[1] = (http_field) {
    .Type = HTTPField_Connection,
    .Value = LitStr("Upgrade"),
  };
  Res.Fields[2] = (http_field) {
    .Type = HTTPField_SecWebSocketAccept,
    .Value = HTTPWebSocketHandshakeCreate(Req, Arena),
  };
  Res.FieldsLen = 3;
  
  return Res;
}

#endif //GS_HTTP_H
