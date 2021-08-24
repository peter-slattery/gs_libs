#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <gl/gl.h>
#include "../src/gs_tests.h"

#include "../src/gs_basic.h"

int main(int ArgCount, char** Args)
{
  TestCtxInitGlobal((gs_test_ctx_desc){ });
  TestGroup((char*)"Compiles for CPP")
  {
    GlobalTest(true);
  }
  return 0;
}