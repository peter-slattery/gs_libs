//
// File: gs_asset_system.h
// Author: Peter Slattery
// Creation Date: 2020-03-23
//
#ifndef GS_ASSET_SYSTEM_H

// DEPENDENCIES
#ifndef GS_TYPES_H
# error "gs_asset_system.h relies on gs_types.h. Please include gs_types.h before gs_asset_system."
#endif

#ifndef GS_OBJ_H
# error "gs_asset_system.h relies on gs_obj.h. Please include gs_obj.h before gs_asset_system."
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
# error "gs_asset_system.h relies on stb_image.h. Please include stb_image.h before gs_asset_system."
#endif

// TODO(Peter): Store all assets a homogonous data
// Create a block_memory_arena, which allocates and frees in blocks of fixed size
// so that you can free memory easily, and all actual asset data goes in there.
// Maintain a free list of blocks so you can treat the block_memory_arena as a singly linked list
// of buffers

struct gsa_text
{
    u32 Length;
    char* Memory;
};

struct gsa_image
{
    u8* Data;
    s32 Width;
    s32 Height;
    s32 Channels;
};

struct gsa_model
{
    u8* VertexBuffer;
    u32 VertexCount;
    u32 VertexBufferSize;
    
    u8* IndexBuffer;
    u32 FacesCount;
    u32 IndexBufferSize;
    
    u32 VertexStride;
    u32 PositionOffset;
    u32 UVOffset;
    u32 NormalOffset;
};

#define GSA_PLATFORM_ALLOC(name) u8* name(u32 Size)
typedef GSA_PLATFORM_ALLOC(gsa_platform_alloc);

#define GSA_PLATFORM_FREE(name) void name(u8* Base, u32 Size)
typedef GSA_PLATFORM_FREE(gsa_platform_free);

#define GSA_PLATFORM_GET_FILE_SIZE(name) u32 name(char* Path)
typedef GSA_PLATFORM_GET_FILE_SIZE(gsa_platform_get_file_size);

#define GSA_PLATFORM_READ_ENTIRE_FILE(name) bool name(char* Path, u8* DestMemory, u32 DestMemorySize)
typedef GSA_PLATFORM_READ_ENTIRE_FILE(gsa_platform_read_entire_file);

#define GSA_INVALID_ASSET_HANDLE 0

enum gsa_asset_type
{
    gsa_AssetType_Invalid,
    gsa_AssetType_gsa_text,
    gsa_AssetType_gsa_image,
    gsa_AssetType_gsa_model,
    gsa_AssetType_Count
};

struct gsa_asset
{
    u32 PathHash;
    gsa_asset_type Type;
    union
    {
        u8* Asset;
        gsa_text* Text;
        gsa_image* Image;
        gsa_model* Model;
    };
};

struct gsa_asset_buffer
{
    gsa_asset* Memory;
};

struct gsa_asset_handle
{
    u32 BufferIndex;
    u32 IndexInBuffer;
};

struct gsa_asset_system
{
    gs_allocator* Allocator;
    gsa_platform_get_file_size* GetFileSize;
    gsa_platform_read_entire_file* ReadEntireFile;
    gs_memory_arena Transient;
    gs_memory_arena AssetMemory;
    gs_dynarray AssetHeaders;
};

static gsa_asset_system
gsa_InitAssetSystem(gs_allocator* Allocator,  gsa_platform_get_file_size* GetFileSize, gsa_platform_read_entire_file* ReadEntireFile)
{
    gsa_asset_system Result = {0};
    Result.Allocator = Allocator;
    Result.GetFileSize = GetFileSize;
    Result.ReadEntireFile = ReadEntireFile;
    Result.Transient = CreateMemoryArena(Allocator);
    Result.AssetMemory = CreateMemoryArena(Allocator);
    Result.AssetHeaders = CreateDynarray(Allocator, gsa_asset, 32);
    stbi_set_flip_vertically_on_load(true);
    return Result;
}

//
// Asset List
//

static gsa_asset*
gsa_GetAssetHeader(gsa_asset_system* AssetSystem, gs_dynarray_handle Handle)
{
    gsa_asset* Asset = GetElement(&AssetSystem->AssetHeaders, gsa_asset, Handle);
    return Asset;
}

static gsa_asset*
gsa_GetAssetHeader(gsa_asset_system* AssetSystem, u32 Index)
{
    Assert(IndexIsValid(AssetSystem->AssetHeaders, Index));
    gs_dynarray_handle Handle = IndexToHandle(&AssetSystem->AssetHeaders, Index);
    return gsa_GetAssetHeader(AssetSystem, Handle);
}

static gsa_asset*
gsa_TakeAsset(gsa_asset_system* AssetSystem, u32* IndexOut)
{
    gs_dynarray_handle AssetHandle = TakeFreeElement(&AssetSystem->AssetHeaders);
    gsa_asset* Result = gsa_GetAssetHeader(AssetSystem, AssetHandle);
    return Result;
}

static gsa_asset*
gsa_TakeAsset(gsa_asset_system* AssetSystem, char* Path, gsa_asset_type Type, u32* IndexOut)
{
    gsa_asset* Asset = gsa_TakeAsset(AssetSystem, IndexOut);
    Asset->PathHash = HashDJB2ToU32(Path);
    Asset->Type = Type;
    return Asset;
}

static void
gsa_MemCopy(u32 Length, u8* From, u8* To)
{
    Assert(From != 0);
    Assert(To != 0);
    
    if (Length == 0) { return; }
    
    for (u32 i = 0; i < Length; i++)
    {
        To[i] = From[i];
    }
}

//
// Text
//

static u32
gsa_CreateTextAsset(gsa_asset_system* AssetSystem, char* Path, u32 FileLength, u8* FileMemory)
{
    Assert(FileLength > 0);
    Assert(FileMemory);
    
    u32 Result = GSA_INVALID_ASSET_HANDLE;
    gsa_asset* TextAsset = gsa_TakeAsset(AssetSystem, Path, gsa_AssetType_gsa_text, &Result);
    
    gs_data AssetMemory = PushSizeToData(&AssetSystem->AssetMemory, sizeof(gsa_text) + FileLength);
    gs_memory_cursor AssetMemoryCursor = CreateMemoryCursor(AssetMemory);
    
    TextAsset->Text = PushStructOnCursor(&AssetMemoryCursor, gsa_text);
    TextAsset->Text->Length = FileLength;
    TextAsset->Text->Memory = PushArrayOnCursor(&AssetMemoryCursor, char, FileLength);
    
    gsa_MemCopy(FileLength, FileMemory, (u8*)TextAsset->Text->Memory);
    
    return Result;
}

//
// Textures
//

static u32
gsa_CreateImageAsset(gsa_asset_system* AssetSystem, char* Path, u32 FileLength, u8* FileMemory)
{
    Assert(FileLength > 0);
    Assert(FileMemory);
    
    u32 Result = GSA_INVALID_ASSET_HANDLE;
    gsa_asset* ImageAsset = gsa_TakeAsset(AssetSystem, Path, gsa_AssetType_gsa_image, &Result);
    
    
    ImageAsset->Image = PushStruct(&AssetSystem->AssetMemory, gsa_image);
    // TODO(Peter): stbi_load actually loads the file again, which we've already done at this stage
    // There should be a variant which just takes in a memory stream
    ImageAsset->Image->Data = stbi_load(Path, &ImageAsset->Image->Width, &ImageAsset->Image->Height, &ImageAsset->Image->Channels, 0);
    Assert(ImageAsset->Image->Data);
    
    return Result;
}

//
// Models
//

static u32
gsa_CreateModelAsset(gsa_asset_system* AssetSystem, char* Path, u32 FileLength, u8* FileMemory)
{
    Assert(FileLength > 0);
    Assert(FileMemory);
    
    u32 Result = GSA_INVALID_ASSET_HANDLE;
    gsa_asset* ModelAsset = gsa_TakeAsset(AssetSystem, Path, gsa_AssetType_gsa_model, &Result);
    
    // TODO(Peter): Memory Management - the gsa_model, the file, and the model data
    // Pull Memory MAnagement out of the obj loader See stb_image for how
    ModelAsset->Model = PushStruct(&AssetSystem->AssetMemory, gsa_model);
    
    gso_compressed_obj Obj = gso_CreateObj(FileMemory, FileLength);
    
    ModelAsset->Model->VertexBuffer = (u8*)Obj.Verteces;
    ModelAsset->Model->VertexCount = Obj.VertexCount;
    ModelAsset->Model->VertexBufferSize = Obj.VertexBufferSize;
    ModelAsset->Model->IndexBuffer = (u8*)Obj.FaceIndices;
    ModelAsset->Model->FacesCount = Obj.FacesCount;
    ModelAsset->Model->IndexBufferSize = Obj.FaceIndicesBufferSize;
    ModelAsset->Model->VertexStride = sizeof(gso_vertex);
    ModelAsset->Model->PositionOffset = 0;
    ModelAsset->Model->UVOffset = 3 * sizeof(r32);
    ModelAsset->Model->NormalOffset = 5 * sizeof(r32);
    
    return Result;
}

//
//
//

#define gsa_GetAsset(assetSystem, index, type) (type*)gsa_GetAssetOfType((assetSystem), (index), gsa_AssetType_##type)
static u8*
gsa_GetAssetOfType(gsa_asset_system* AssetSystem, u32 Index, gsa_asset_type Type)
{
    u8* Result = 0;
    gsa_asset* Asset = gsa_GetAssetHeader(AssetSystem, Index);
    Assert(Asset->Type = Type);
    Result = Asset->Asset;
    return Result;
}

static u8*
gsa_GetAssetOfType(gsa_asset_system* AssetSystem, char* Path, gsa_asset_type Type)
{
    u8* Result = 0;
    u32 PathHash = HashDJB2ToU32(Path);
    s32 AssetIndex = -1;
    gsa_asset* AssetHeader = 0;
    for (u32 i = 1; i <= AssetSystem->AssetHeaders.ElementCount; i++)
    {
        gsa_asset* CheckAsset = gsa_GetAssetHeader(AssetSystem, i);
        if (CheckAsset->PathHash == PathHash)
        {
            AssetIndex = i;
            AssetHeader = CheckAsset;
        }
    }
    
    if (AssetIndex >= 0)
    {
        Result = AssetHeader->Asset;
    }
    else
    {
        u32 FileLength = AssetSystem->GetFileSize(Path);
        u8* FileMemory = PushSize(&AssetSystem->Transient, FileLength);
        if (AssetSystem->ReadEntireFile(Path, FileMemory, FileLength))
        {
            if (FileLength > 0 && FileMemory)
            {
                u32 AssetHandle = 0;
                switch (Type)
                {
                    case gsa_AssetType_gsa_text:
                    {
                        AssetHandle = gsa_CreateTextAsset(AssetSystem, Path, FileLength, FileMemory);
                    }break;
                    
                    case gsa_AssetType_gsa_image:
                    {
                        AssetHandle = gsa_CreateImageAsset(AssetSystem, Path, FileLength, FileMemory);
                    }break;
                    
                    case gsa_AssetType_gsa_model:
                    {
                        AssetHandle = gsa_CreateModelAsset(AssetSystem, Path, FileLength, FileMemory);
                    }break;
                }
                
                if (AssetHandle)
                {
                    Result = gsa_GetAssetOfType(AssetSystem, AssetHandle, Type);
                }
            } else {
                InvalidCodePath;
            }
        }
        else
        {
            InvalidCodePath;
        }
        ClearArena(&AssetSystem->Transient);
    }
    return Result;
}

//
// Cleanup
//

static void
gsa_CleanupAssetSystem(gsa_asset_system* AssetSystem)
{
    FreeMemoryArena(&AssetSystem->Transient);
    FreeMemoryArena(&AssetSystem->AssetMemory);
    FreeDynarray(&AssetSystem->AssetHeaders);
}

#define GS_ASSET_SYSTEM_H
#endif // GS_ASSET_SYSTEM_H