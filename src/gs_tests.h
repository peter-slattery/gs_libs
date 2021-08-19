/* date = May 10th 2021 6:46 pm */

#ifndef GS_TESTS_H
#define GS_TESTS_H

typedef struct gs_test_ctx_desc
{
  int LinePadMax;
} gs_test_ctx_desc;

typedef struct gs_test_ctx
{
  gs_test_ctx_desc Desc;
  
  int TestsCount;
  int TestsPassed;
  
  char* CurrGroupName;
} gs_test_ctx;

static gs_test_ctx GlobalTestCtx = {0};

static gs_test_ctx 
TestCtxCreate(gs_test_ctx_desc Desc)
{
  gs_test_ctx Result = {0};
  Result.Desc = Desc;
  if (Result.Desc.LinePadMax == 0) {
    Result.Desc.LinePadMax = 45;
  }
  return Result;
}

static void 
TestCtxInitGlobal(gs_test_ctx_desc Desc)
{
  GlobalTestCtx = TestCtxCreate(Desc);
}

static void 
BeginTestGroup(gs_test_ctx* Ctx, char* GroupName)
{
  int GroupNameLength = strlen(GroupName);
  int PadLength = Ctx->Desc.LinePadMax - GroupNameLength;
  if (PadLength < 0) { PadLength = 0; }
  
  printf("\"%s\" ", GroupName);
  for (int i = 0; i < PadLength; i++)
  {
    printf("-");
  }
  printf(" [");
  
  Ctx->TestsCount = 0;
  Ctx->TestsPassed = 0;
  Ctx->CurrGroupName = GroupName;
}

static void 
EndTestGroup(gs_test_ctx* Ctx, char* GroupName)
{
  if (strcmp(GroupName, Ctx->CurrGroupName) != 0)
  {
    printf("Error: A different test group is being closed than the one that is currently open.\n");
    printf("\tCurrently Open: %s\n", Ctx->CurrGroupName);
    printf("\tAttempting Close: %s\n", GroupName);
    return;
  }
  
  int SpacesCount = 25 - Ctx->TestsCount;
  if (SpacesCount < 0) SpacesCount = 0;
  
  printf("]");
  for (int i = 0; i < SpacesCount; i++)
  {
    printf(" ");
  }
  printf(" [%02i/%02i] (passed / total) ",
         Ctx->TestsPassed, Ctx->TestsCount);
  
  if (Ctx->TestsPassed == Ctx->TestsCount)
  {
    printf("SUCCESS ( )\n");
  } else {
    printf("FAILED  (X)\n");
  }
}

static void
Test(gs_test_ctx* Ctx, bool Result)
{
  Ctx->TestsCount += 1;
  if (Result) Ctx->TestsPassed += 1;
  char* StatusStr = (char*)(Result ? "." : "X");
  printf("%s", StatusStr);
}

static void 
BeginGlobalTestGroup(char* GroupName)
{
  BeginTestGroup(&GlobalTestCtx, GroupName);
}

static void 
EndGlobalTestGroup (char* GroupName)
{
  EndTestGroup(&GlobalTestCtx, GroupName);
}

static void 
GlobalTest(bool Result)
{
  Test(&GlobalTestCtx, Result);
}

#define TestGroup(name) \
for (int _i_ = (BeginGlobalTestGroup(name), 0); !_i_; _i_ += 1, EndGlobalTestGroup(name))



#endif //GS_TESTS_H
