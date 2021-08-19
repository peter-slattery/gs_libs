/* date = May 22nd 2021 2:10 pm */

#ifndef GS_SCRATCH_CTX_H
#define GS_SCRATCH_CTX_H

typedef struct gs_ctx gs_ctx;

#define GS_CTX_CLEANUP(name) void name(gs_ctx* Ctx)
typedef GS_CTX_CLEANUP(gs_ctx_cleanup);

typedef struct gs_ctx
{
    gs_allocator* Allocator;
    gs_file_handler* FileHandler;
    
    gs_ctx_cleanup* CtxCleanup;
    u8* UserData;
    
    gs_ctx* Parent;
} gs_ctx;

typedef struct gs_ctx_desc
{
    gs_allocator* Allocator;
    gs_file_handler* FileHandler;
    gs_ctx_cleanup* CtxCleanup;
    u8* UserData;
} gs_ctx_desc;

typedef struct gs_ctx_env
{
    gs_ctx* CtxList;
    gs_ctx* FreeCtxPool;
} gs_ctx_env;

global gs_ctx_env* Ctx_;

internal gs_ctx 
CtxGet()
{
    Assert(Ctx_);
    gs_ctx Result = {0};
    if (Ctx_->CtxList)
    {
        Result = *Ctx_->CtxList;
    }
    return Result;
}

internal void
CtxPush(gs_ctx* NewCtx)
{
    Assert(Ctx_);
    
    // Fill any missing parameters with value from the parent
    if (Ctx_->CtxList)
    {
        gs_ctx CurrCtx = CtxGet();
        
        // @MetaOp: be able to say 'for(field in Ctx_)`
        if (!NewCtx->Allocator) 
        {
            NewCtx->Allocator = CurrCtx.Allocator;
        }
        if (!NewCtx->FileHandler) 
        {
            NewCtx->FileHandler = CurrCtx.FileHandler;
        }
        if (!NewCtx->CtxCleanup)
        {
            NewCtx->CtxCleanup = CurrCtx.CtxCleanup;
        }
        if (!NewCtx->UserData)
        {
            NewCtx->UserData = CurrCtx.UserData;
        }
        
        NewCtx->Parent = Ctx_->CtxList;
        Ctx_->CtxList = NewCtx;
    } 
    else 
    {
        Ctx_->CtxList = NewCtx;
    }
}

internal void
CtxPop()
{
    gs_ctx* CurrCtx = Ctx_->CtxList;
    Ctx_->CtxList = CurrCtx->Parent;
    
    if (CurrCtx->CtxCleanup)
    {
        CurrCtx->CtxCleanup(CurrCtx);
    }
    
    if (Ctx_->FreeCtxPool)
    {
        CurrCtx->Parent = Ctx_->FreeCtxPool;
    }
    Ctx_->FreeCtxPool = CurrCtx;
}

internal void 
CtxEnter(gs_ctx_desc* Desc)
{
    gs_ctx* NewCtx = 0;
    
    if (!Ctx_)
    {
        Assert(Desc->Allocator);
        Ctx_ = AllocStruct(*Desc->Allocator, gs_ctx_env);
    }
    
    if (Ctx_->FreeCtxPool)
    {
        NewCtx = Ctx_->FreeCtxPool;
        Ctx_->FreeCtxPool = NewCtx->Parent;
        NewCtx->Parent = 0;
    }
    
    if (!NewCtx)
    {
        gs_allocator* Allocator = CtxGet().Allocator;
        if (!Allocator) Allocator = Desc->Allocator;
        Assert(Allocator);
        NewCtx = AllocStruct(*Allocator, gs_ctx);
    }
    
    *NewCtx = (gs_ctx){
        .Allocator = Desc->Allocator,
        .FileHandler = Desc->FileHandler,
        .CtxCleanup = Desc->CtxCleanup,
        .UserData = Desc->UserData,
    };
    
    CtxPush(NewCtx);
}

internal void
CtxExit()
{
    CtxPop();
}


int main (int ArgCount, char** Args)
{
    // Thread Context
    
    test_struct TA = { .Foo = 1337, };
    test_struct TB = { .Foo = 1873, };
    
    
    gs_file_handler FH = CreatePlatformFileHandler(A);
    CtxEnter(&(gs_ctx_desc){
                 .Allocator = &A,
                 .FileHandler = &FH,
                 .UserData = (u8*)&TA,
             });
    {
        gs_ctx Ctx = CtxGet();
        gs_data C = AllocData(*Ctx.Allocator, 256);
        Assert(C.Size == 256);
        
        CtxEnter(&(gs_ctx_desc){0});
        {
            Ctx = CtxGet();
            C = AllocData(*Ctx.Allocator, 512);
            Assert(C.Size == 512);
            Assert(((test_struct*)Ctx.UserData)->Foo == 1337);
            
            CtxEnter(&(gs_ctx_desc){
                         .UserData = (u8*)&TB
                     });
            {
                Ctx = CtxGet();
                Assert(((test_struct*)Ctx.UserData)->Foo == 1873);
            }
            CtxExit();
            
            Ctx = CtxGet();
            Assert(((test_struct*)Ctx.UserData)->Foo == 1337);
        }
        CtxExit();
    }
    CtxExit();
    
    
}
#endif //GS_SCRATCH_CTX_H
