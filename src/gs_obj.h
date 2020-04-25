//
// File: gs_obj.h
// Author: Peter Slattery
// Creation Date: 2020-03-15
//
#ifndef GS_OBJ_H

#ifndef GS_TYPES_H
# error "gs_obj.h relies on gs_types.h. Please include gs_types.h before gs_obj.h"
#endif

enum gso_vertex_packing
{
    gso_VertexPacking_Position = 1 << 0,
    gso_VertexPacking_UV = 1 << 1,
    gso_VertexPacking_Normal = 1 << 2,
};

// The # of floats in each element
#define gso_VertexEleSize_Position 3
#define gso_VertexEleSize_UV 2
#define gso_VertexEleSize_Normal 3

union gso_v2
{
    struct {
        r32 X;
        r32 Y;
    };
    r32 E[2];
};

union gso_v3
{
    struct {
        r32 X;
        r32 Y;
        r32 Z;
    };
    r32 E[3];
};

struct gso_vertex_indices
{
    s32 Position;
    s32 UV;
    s32 Normal;
};

struct gso_expanded_obj
{
    // NOTE(Peter): This buffer contains all the data relevant to the obj
    // stored as:
    //     [ pos, pos, uv, uv, norm, norm, pos_index, pos_index ]
    // The 4 buffers below are pointers into this buffer, and are ways to
    // access the arrays of each component.
    u8* BufferBase;
    u32 BufferSize;
    
    u32 PositionsCount;
    u32 UVsCount;
    u32 NormalsCount;
    // :OnlyTris
    u32 FacesCount;
    u32 IndicesCount;
    
    gso_v3* Positions;
    gso_v2* UVs;
    gso_v3* Normals;
    gso_vertex_indices* Indices;
    
    b32 Smooth;
};

struct gso_face_indices
{
    u32 Indices[3];
};

struct gso_vertex
{
    r32 Position[3];
    r32 UV[2];
    r32 Normal[3];
};

struct gso_compressed_obj
{
    gso_vertex* Verteces;
    u32 VertexCount;
    u32 VertexBufferSize;
    
    gso_face_indices* FaceIndices;
    u32 FacesCount;
    u32 FaceIndicesBufferSize;
};

#define GSO_STRING_BUFFER_SIZE_INCREMENT 256
struct gso_string_buffer
{
    char* Memory;
    u32 Size;
    u32 Used;
};

struct gso_tokenizer
{
    char* File;
    u32 FileSize;
    char* At;
    u32 TokenLength;
};

static r32
gso_Pow(r32 N, s32 Power)
{
    r32 Result = N;
    for(s32 i = 1; i < Power; i++) { Result *= N; }
    return Result;
}

static u32
gso_Min(u32 A, u32 B)
{
    u32 Result = A;
    if (B < A) { Result = B; }
    return Result;
}

static r32
gso_Abs(r32 Value)
{
    r32 Result = Value;
    if (Value < 0)
    {
        Result = -1 * Value;
    }
    return Result;
}

static u32
gso_StringLength(char* String)
{
    u32 Result = 0;
    char* At = String;
    while (At[0] != 0)
    {
        At++;
    }
    Result = At - String;
    return Result;
}

static b32
gso_IsNewline(char C)
{
    b32 Result = ((C == '\n') || (C == '\r'));
    return Result;
}

static b32
gso_IsWhitespace(char C)
{
    b32 Result = ((C == ' ') || (C == '\t'));
    return Result;
}

static b32
gso_IsNumeric(char C)
{
    b32 Result = (C >= '0' && C <= '9');
    return Result;
}

static b32
gso_TokenizerIsValid(gso_tokenizer T)
{
    b32 Result = ((T.At - T.File) < T.FileSize && T.At[0]);
    return Result;
}

static b32
gso_CharAtEquals(gso_tokenizer T, char C)
{
    b32 Result = T.At[0] == C;
    return Result;
}

static b32
gso_TokenAtEquals(gso_tokenizer T, char* Token)
{
    b32 Result = false;
    u32 TokenLength = gso_StringLength(Token);
    if (T.TokenLength == TokenLength)
    {
        Result = true;
        char* TAt = T.At;
        char* TokenAt = Token;
        for (u32 i = 0; i < TokenLength; i++)
        {
            if (TAt[0] != TokenAt[0])
            {
                Result = false;
                break;
            }
            TAt++;
            TokenAt++;
        }
    }
    return Result;
}

static void
gso_EatWhitespace(gso_tokenizer* T)
{
    while (gso_TokenizerIsValid(*T) && gso_IsWhitespace(T->At[0]))
    {
        T->At++;
    }
}

inline b32
gso_CharIsTokenDelimiter(char C)
{
    b32 Result = (C == ' ') || (C == '\n') || (C == '\t') || (C == '\r') || (C == 0);
    return Result;
}

inline u32
gso_GetCurrentTokenLength(gso_tokenizer T)
{
    u32 Result = 0;
    char* TokenEnd = T.At;
    while (*TokenEnd && (TokenEnd - T.File) < T.FileSize && !gso_CharIsTokenDelimiter(TokenEnd[0]))
    {
        TokenEnd++;
    }
    Result = TokenEnd - T.At;
    return Result;
}

static void
gso_NextToken(gso_tokenizer* T)
{
    T->At += T->TokenLength;
    gso_EatWhitespace(T);
    T->TokenLength = gso_GetCurrentTokenLength(*T);
}

static void
gso_EatToNextLine(gso_tokenizer* T)
{
    while (gso_TokenizerIsValid(*T) && !gso_IsNewline(T->At[0]))
    {
        T->At++;
    }
    while (gso_TokenizerIsValid(*T) && gso_IsNewline(T->At[0]))
    {
        T->At++;
    }
    T->TokenLength = gso_GetCurrentTokenLength(*T);
}

static gso_tokenizer
gso_CreateTokenizer(char* File, u32 FileSize)
{
    gso_tokenizer Tokenizer = {0};
    Tokenizer.File = File;
    Tokenizer.FileSize = FileSize;
    Tokenizer.At = Tokenizer.File;
    Tokenizer.TokenLength = gso_GetCurrentTokenLength(Tokenizer);
    return Tokenizer;
}

static u32
gso_CharToUInt(char C)
{
    u32 Result = C - '0';
    return Result;
}

static char*
gso_EatToNextLine(char* FileAt, char* File, s32 FileSize)
{
    while ((FileAt - File < FileSize) && *FileAt && !gso_IsNewline(*FileAt)) { FileAt++; }
    while ((FileAt - File < FileSize) && *FileAt && gso_IsNewline(*FileAt)) { FileAt++; }
    return FileAt;
}

static char*
gso_EatWhitespace(char* FileAt, char* File, s32 FileSize)
{
    while ((FileAt - File < FileSize) && *FileAt && gso_IsWhitespace(*FileAt)) { FileAt++; }
    return FileAt;
}

static void
gso_ParseInt(gso_tokenizer* T, s32* OutInt)
{
    u32 ResultValue = 0;
    char* At = T->At;
    while (*At && (At - T->File) < T->FileSize && gso_IsNumeric(*At))
    {
        ResultValue = gso_CharToUInt(At[0]) + (ResultValue * 10);
        At++;
    }
    *OutInt = ResultValue;
    T->At = At;
    T->TokenLength = gso_GetCurrentTokenLength(*T);
}

static void
gso_ParseFloat(gso_tokenizer* T, r32* OutFloat)
{
    s32 Negative = 1;
    float ResultValue = 0;
    
    u32 Parsed = 0;
    if (T->At[Parsed] == '-') {
        Parsed++;
        Negative = -1;
    }
    for (; Parsed < T->TokenLength; Parsed++)
    {
        if (gso_IsNumeric(T->At[Parsed]))
        {
            ResultValue = (float)gso_CharToUInt(T->At[Parsed]) + (ResultValue * 10);
        }
        else if (T->At[Parsed] == '.' || T->At[Parsed] == 0)
        {
            break;
        }
    }
    if (T->At[Parsed] == '.')
    {
        Parsed++;
        float AfterPoint = 0;
        s32 PlacesAfterPoint = 0;
        for(; Parsed < T->TokenLength; Parsed++)
        {
            AfterPoint = (float)gso_CharToUInt(T->At[Parsed]) + (AfterPoint * 10);
            PlacesAfterPoint++;
        }
        AfterPoint = AfterPoint / gso_Pow(10, PlacesAfterPoint);
        ResultValue += AfterPoint;
    }
    *OutFloat = ResultValue * Negative;
    gso_NextToken(T);
}

static void
gso_ParseVertexList(gso_tokenizer* T, u32 ListLength, float* OutPos)
{
    gso_EatWhitespace(T);
    for (u32 i = 0; i < ListLength; i++)
    {
        gso_ParseFloat(T, OutPos + i);
    }
    gso_EatToNextLine(T);
}

static void
gso_ParseTuple(gso_tokenizer* T, gso_vertex_indices* Face)
{
    if (gso_IsNumeric(*T->At))
    {
        gso_ParseInt(T, &Face->Position);
        Face->Position -= 1;
        Assert(Face->Position >= 0);
    }
    else
    {
        Face->Position = 0;
    }
    if (*T->At == '/') { T->At++; }
    
    if (gso_IsNumeric(*T->At))
    {
        gso_ParseInt(T, &Face->UV);
        Face->UV -= 1;
        Assert(Face->UV >= 0);
    }
    else
    {
        Face->UV = 0;
    }
    
    if (*T->At == '/') { T->At++; }
    
    if (gso_IsNumeric(*T->At))
    {
        gso_ParseInt(T, &Face->Normal);
        Face->Normal -= 1;
        Assert(Face->Normal >= 0);
    }
    else
    {
        Face->Normal = 0;
    }
}

#define gso_PositionBufferSize(count) ((count) * 3)
#define gso_UVBufferSize(count) ((count) * 2)
#define gso_NormalBufferSize(count) ((count) * 3)
#define gso_FaceIndexBufferSize(count, vertsPerFace) ((count) * (vertsPerFace))
#define gso_VertexBufferSize(obj) (gso_PositionBufferSize((obj).VertexPositionsCount) + gso_UVBufferSize((obj).VertexUVsCount) + gso_NormalBufferSize((obj).VertexNormalsCount))

static gso_expanded_obj
gso_PreprocessObjFile(u8* FileData, u32 FileSize)
{
    gso_expanded_obj Result = {0};
    
    gso_tokenizer Tokenizer = gso_CreateTokenizer((char*)FileData, FileSize);
    while (gso_TokenizerIsValid(Tokenizer))
    {
        if (gso_CharAtEquals(Tokenizer, '#'))
        {
            gso_EatToNextLine(&Tokenizer);
        }
        else if (gso_TokenAtEquals(Tokenizer, "v"))
        {
            gso_NextToken(&Tokenizer);
            Result.PositionsCount += 1;
            gso_EatToNextLine(&Tokenizer);
        }
        else if (gso_TokenAtEquals(Tokenizer, "vt"))
        {
            gso_NextToken(&Tokenizer);
            Result.UVsCount += 1;
            gso_EatToNextLine(&Tokenizer);
        }
        else if (gso_TokenAtEquals(Tokenizer, "vn"))
        {
            gso_NextToken(&Tokenizer);
            Result.NormalsCount += 1;
            gso_EatToNextLine(&Tokenizer);
        }
        else if (gso_TokenAtEquals(Tokenizer, "f"))
        {
            gso_NextToken(&Tokenizer);
            Result.FacesCount += 1;
            gso_EatToNextLine(&Tokenizer);
        }
        else
        {
            gso_EatToNextLine(&Tokenizer);
        }
    }
    
    Result.BufferSize = sizeof(r32) * gso_PositionBufferSize(Result.PositionsCount);
    Result.BufferSize += sizeof(r32) * gso_UVBufferSize(Result.UVsCount);
    Result.BufferSize += sizeof(r32) * gso_NormalBufferSize(Result.NormalsCount);
    Result.BufferSize += sizeof(gso_vertex_indices) * gso_FaceIndexBufferSize(Result.FacesCount, 3);
    
    return Result;
}

static u32
gso_ParseObj(u8* FileData, u32 FileSize, gso_expanded_obj* Obj)
{
    u32 Result = 0;
    // NOTE(Peter): Must provide an Obj struct with a valid buffer
    Assert(Obj->BufferBase != 0);
    
    u32 PositionsSize = Obj->PositionsCount * sizeof(gso_v3);
    u32 UVsSize = Obj->UVsCount * sizeof(gso_v2);
    u32 NormalsSize = Obj->NormalsCount * sizeof(gso_v3);
    
    Obj->Positions = (gso_v3*)Obj->BufferBase;
    Obj->UVs = (gso_v2*)(Obj->BufferBase + PositionsSize);
    Obj->Normals = (gso_v3*)(Obj->BufferBase + PositionsSize + UVsSize);
    Obj->Indices = (gso_vertex_indices*)(Obj->BufferBase + PositionsSize + UVsSize + NormalsSize);
    
    gso_tokenizer Tokenizer = gso_CreateTokenizer((char*)FileData, FileSize);
    u32 PositionsCount = 0;
    u32 UVsCount = 0;
    u32 NormalsCount = 0;
    u32 FacesCount = 0;
    while (gso_TokenizerIsValid(Tokenizer))
    {
        if (gso_TokenAtEquals(Tokenizer, "#"))
        {
            gso_EatToNextLine(&Tokenizer);
        }
        else if (gso_TokenAtEquals(Tokenizer, "v"))
        {
            gso_NextToken(&Tokenizer);
            Assert(PositionsCount < Obj->PositionsCount);
            u32 NewVertexIndex = PositionsCount++;
            gso_ParseVertexList(&Tokenizer, 3, &Obj->Positions[NewVertexIndex].E[0]);
        }
        else if (gso_TokenAtEquals(Tokenizer, "vt"))
        {
            gso_NextToken(&Tokenizer);
            Assert(UVsCount < Obj->UVsCount);
            u32 NewUVIndex = UVsCount++;
            gso_ParseVertexList(&Tokenizer, 2, &Obj->UVs[NewUVIndex].E[0]);
        }
        else if (gso_TokenAtEquals(Tokenizer, "vn"))
        {
            gso_NextToken(&Tokenizer);
            Assert(NormalsCount < Obj->NormalsCount);
            u32 NewNormalIndex = NormalsCount++;
            gso_ParseVertexList(&Tokenizer, 3, &Obj->Normals[NewNormalIndex].E[0]);
        }
        else if (gso_TokenAtEquals(Tokenizer, "f"))
        {
            gso_NextToken(&Tokenizer);
            Assert(FacesCount < Obj->FacesCount);
            u32 NewFaceIndex = gso_FaceIndexBufferSize(FacesCount++, 3);
            u32 V = 0;
            while (gso_TokenizerIsValid(Tokenizer) && !gso_IsNewline(Tokenizer.At[0]))
            {
                // NOTE(Peter): This only handles triangles at the moment,
                // TODO(Peter): handle quads - need some way to triangulate them.
                // :OnlyTris
                Assert(V < 3);
                gso_EatWhitespace(&Tokenizer);
                gso_ParseTuple(&Tokenizer, Obj->Indices + NewFaceIndex + V);
                V += 1;
            }
            gso_EatToNextLine(&Tokenizer);
        }
        else
        {
            gso_EatToNextLine(&Tokenizer);
        }
    }
    return Result;
}

static gso_face_indices*
gso_TakeFaceFromModel(gso_compressed_obj* Obj, u32 FacesMax)
{
    Assert(Obj->FacesCount < FacesMax);
    gso_face_indices* Result = Obj->FaceIndices + Obj->FacesCount++;
    return Result;
}

static void
gso_CopyFloatArray(u32 Count, r32* Src, r32* Dest)
{
    for (u32 i = 0; i < Count; i++)
    {
        Dest[i] = Src[i];
    }
}

static gso_compressed_obj
gso_PrepareCompressedObj(gso_expanded_obj ExpandedObj)
{
    gso_compressed_obj CompObj = {0};
    u32 FacesCount = ExpandedObj.FacesCount;
    u32 VertexCount = FacesCount * 3;
    CompObj.VertexBufferSize = sizeof(gso_vertex) * VertexCount;
    CompObj.FaceIndicesBufferSize = sizeof(gso_face_indices) * FacesCount;
    return CompObj;
}

static void
gso_CompressObj(gso_expanded_obj ExpandedObj, gso_compressed_obj* CompObj)
{
    // NOTE(Peter): At the moment, this isn't attempting to reduce the nubmer of unique vertecies
    // in the compressed obj. Technically this means its not compressed yet, but this has been
    // pissing me off so I just want to make some progress. Will come back later and ensure each
    // vertex is a unique combination of position, normal, and uv
    
    // :OnlyTris
    u32 FacesCount = ExpandedObj.FacesCount;
    u32 VertexCount = FacesCount * 3;
    
    for (u32 Face = 0; Face < ExpandedObj.FacesCount; Face++)
    {
        gso_face_indices* CompFace = gso_TakeFaceFromModel(CompObj, FacesCount);
        
        // :OnlyTris
        for (u32 V = 0; V < 3; V++)
        {
            gso_vertex_indices ExVert = ExpandedObj.Indices[(Face * 3) + V];
            gso_v3 ExpandedPositions = ExpandedObj.Positions[ExVert.Position];
            gso_v2 ExpandedUVs = ExpandedObj.UVs[ExVert.UV];
            gso_v3 ExpandedNormals = ExpandedObj.Normals[ExVert.Normal];
            
            Assert(CompObj->VertexCount < VertexCount);
            u32 CompVertexIndex = CompObj->VertexCount++;
            gso_CopyFloatArray(3, &ExpandedPositions.E[0], CompObj->Verteces[CompVertexIndex].Position);
            gso_CopyFloatArray(2, &ExpandedUVs.E[0], CompObj->Verteces[CompVertexIndex].UV);
            gso_CopyFloatArray(3, &ExpandedNormals.E[0], CompObj->Verteces[CompVertexIndex].Normal);
            
            CompFace->Indices[V] = CompVertexIndex;
        }
    }
}

static gso_compressed_obj
gso_CreateObj(gs_data Memory)
{
    gso_compressed_obj Result = {0};
    
    gso_expanded_obj CubeObj = gso_PreprocessObjFile(Memory.Memory, Memory.Size);
    CubeObj.BufferBase = (u8*)malloc(CubeObj.BufferSize);
    gso_ParseObj(Memory.Memory, Memory.Size, &CubeObj);
    
    Result = gso_PrepareCompressedObj(CubeObj);
    Result.Verteces = (gso_vertex*)malloc(Result.VertexBufferSize);
    Result.FaceIndices = (gso_face_indices*)malloc(Result.FaceIndicesBufferSize);
    gso_CompressObj(CubeObj, &Result);
    
    free(CubeObj.BufferBase);
    return Result;
}

// Writing Obj Files
static u32
gso_CharArrayLength(char* Arr)
{
    u32 Result = 0;
    char* At = Arr;
    while (*At)
    {
        At++;
    }
    Result = At - Arr;
    return Result;
}

static void
gso_PushCharOnBuffer(gso_string_buffer* Dest, char C)
{
    if (Dest->Used + 1 > Dest->Size)
    {
        Dest->Size += GSO_STRING_BUFFER_SIZE_INCREMENT;
        Dest->Memory = (char*)realloc((u8*)Dest->Memory, Dest->Size);
    }
    Dest->Memory[Dest->Used++] = C;
}

static void
gso_WriteToBuffer(gso_string_buffer* Dest, char* ToWrite)
{
    u32 WriteLength = gso_CharArrayLength(ToWrite);
    if (Dest->Used + WriteLength > Dest->Size)
    {
        Dest->Size += gso_Min(GSO_STRING_BUFFER_SIZE_INCREMENT, WriteLength);
        Dest->Memory = (char*)realloc((u8*)Dest->Memory, Dest->Size);
    }
    for (u32 i = 0; i < WriteLength; i++)
    {
        Dest->Memory[Dest->Used++] = ToWrite[i];
    }
}

static void
gso_WriteIntToBuffer(gso_string_buffer* Dest, s32 Value)
{
    char TempBuffer[32];
    u32 TempBufferUsed = 0;
    s32 Remaining = Value;
    b32 Negative = false;
    if (Remaining < 0)
    {
        Negative = true;
        Remaining *= -1;
    }
    if (Remaining > 0)
    {
        while (Remaining > 0)
        {
            char CharValue = '0' + (Remaining % 10);
            TempBuffer[TempBufferUsed++] = CharValue;
            Remaining /= 10;
        }
        if (Remaining)
        {
            gso_PushCharOnBuffer(Dest, '-');
        }
        for (u32 i = 1; i <= TempBufferUsed; i++)
        {
            gso_PushCharOnBuffer(Dest, TempBuffer[(TempBufferUsed - i)]);
        }
    }
    else
    {
        gso_PushCharOnBuffer(Dest, '0');
    }
}

static void
gso_WriteFloatToBuffer(gso_string_buffer* Dest, r32 Value)
{
    s32 IntPart = (s32)Value;
    r32 FractPart = gso_Abs(Value - (float)IntPart);
    gso_WriteIntToBuffer(Dest, IntPart);
    gso_PushCharOnBuffer(Dest, '.');
    s32 FractPartInt = FractPart * gso_Pow(10, 5);
    gso_WriteIntToBuffer(Dest, FractPartInt);
}

static gso_string_buffer
gso_WriteObj(gso_expanded_obj Obj)
{
    gso_string_buffer Result = {0};
    gso_WriteToBuffer(&Result, "# Greensleevless Object File Parser\n");
    gso_WriteToBuffer(&Result, "# www.peter-slattery.com\n");
    
    for (int i = 0; i < Obj.PositionsCount; i++)
    {
        gso_WriteToBuffer(&Result, "v ");
        gso_WriteFloatToBuffer(&Result, Obj.Positions[i].X);
        gso_WriteToBuffer(&Result, " ");
        gso_WriteFloatToBuffer(&Result, Obj.Positions[i].Y);
        gso_WriteToBuffer(&Result, " ");
        gso_WriteFloatToBuffer(&Result, Obj.Positions[i].Z);
        gso_WriteToBuffer(&Result, "\n");
    }
    
    for (int i = 0; i < Obj.UVsCount; i++)
    {
        gso_WriteToBuffer(&Result, "vt ");
        gso_WriteFloatToBuffer(&Result, Obj.UVs[i].X);
        gso_WriteToBuffer(&Result, " ");
        gso_WriteFloatToBuffer(&Result, Obj.UVs[i].Y);
        gso_WriteToBuffer(&Result, "\n");
    }
    
    for (int i = 0; i < Obj.NormalsCount; i++)
    {
        gso_WriteToBuffer(&Result, "vn ");
        gso_WriteFloatToBuffer(&Result, Obj.Normals[i].X);
        gso_WriteToBuffer(&Result, " ");
        gso_WriteFloatToBuffer(&Result, Obj.Normals[i].Y);
        gso_WriteToBuffer(&Result, " ");
        gso_WriteFloatToBuffer(&Result, Obj.Normals[i].Z);
        gso_WriteToBuffer(&Result, "\n");
    }
    
    gso_WriteToBuffer(&Result, "s off\n");
    
    for (int i = 0; i < Obj.FacesCount; i++)
    {
        gso_WriteToBuffer(&Result, "f ");
        for (int j = 0; j < 3; j++)
        {
            gso_WriteIntToBuffer(&Result, Obj.Indices[(i * 3) + j].Position);
            gso_WriteToBuffer(&Result, "/");
            gso_WriteIntToBuffer(&Result, Obj.Indices[(i * 3) + j].UV);
            gso_WriteToBuffer(&Result, "/");
            gso_WriteIntToBuffer(&Result, Obj.Indices[(i * 3) + j].Normal);
            gso_PushCharOnBuffer(&Result, ' ');
        }
        gso_WriteToBuffer(&Result, "\n");
    }
    gso_PushCharOnBuffer(&Result, '\0');
    
    return Result;
}

#define GS_OBJ_H
#endif // GS_OBJ_H