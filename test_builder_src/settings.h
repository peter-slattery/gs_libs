/* date = May 10th 2021 2:47 pm */

#ifndef SETTINGS_H
#define SETTINGS_H

// Win32

char* SetupCommands_Win32[] = {
  "@echo off\n",
  "SET MyPath=%~dp0\n",
  "SET BinPath=%MyPath:~0,-1%\n",
  "SET ProjectPath=%BinPath%\\..\n",
  "IF NOT EXIST %ProjectPath%\\tests_run_tree mkdir %ProjectPath%\\tests_run_tree\n",
  "set CompilerFlags=-O0 -g\n",
  "set LinkerLibraries=user32.lib gdi32.lib msvcrt.lib msvcprt.lib opengl32.lib\n",
  "pushd %ProjectPath%\\tests_run_tree \n\n",
};
int SetupCommandsLength_Win32 = CArrayLength(SetupCommands_Win32);

// for each test, you have access to
//   TestFileFullPath
//   TestFileNoExtensions
char* BuildCommands_Win32[] = {
  "clang %CompilerFlags% -c %TestFileFullPath%\n",
  "link /NOLOGO -debug %TestFileNoExtensions%.o %LinkerLibraries%\n",
  "%TestFileNoExtensions%.exe\n",
};
int BuildCommandsLength_Win32 = CArrayLength(BuildCommands_Win32);

char* CleanupCommands_Win32[] = {
  "popd\n",
};
int CleanupCommandsLength_Win32 = CArrayLength(CleanupCommands_Win32);

// OSX

char* SetupCommands_OSX[] = {
  "mkdir /Users/jrinker/_ps/projects/gs_libs/tests_run_tree\n",
  "CompilerFlags=-O0 -g\n",
  "pushd /Users/jrinker/_ps/projects/gs_libs/tests_run_tree \n\n",
};
int SetupCommandsLength_OSX = CArrayLength(SetupCommands_OSX);

// for each test, you have access to
//   TestFileFullPath
//   TestFileNoExtensions
char* BuildCommands_OSX[] = {
  "clang $TestFileFullPath -o $TestFileNoExtensions\n",
  "chmod +x $TestFileNoExtensions"
    "$TestFileNoExtensions\n",
};
int BuildCommandsLength_OSX = CArrayLength(BuildCommands_OSX);

char* CleanupCommands_OSX[] = {
  "popd\n",
};
int CleanupCommandsLength_OSX = CArrayLength(CleanupCommands_OSX);

// WASM

char* SetupCommands_WASM[] = {
  "@echo off\n",
  "SET MyPath=%~dp0\n",
  "SET BinPath=%MyPath:~0,-1%\n",
  "SET ProjectPath=%BinPath%\\..\n",
  "IF NOT EXIST %ProjectPath%\\tests_run_tree mkdir %ProjectPath%\\tests_run_tree\n",
  "IF NOT EXIST %ProjectPath%\\tests_run_tree\\wasm mkdir %ProjectPath%\\tests_run_tree\\wasm\n",
  "set CompilerFlags=-c -target wasm32 -nostdinc -D__EMSCRIPTEN__ -D_LIBCPP_ABI_VERSION=2 -fvisibility=hidden -fno-builtin -fno-exceptions -fno-threadsafe-statics -MMD -MP -Ofast\n",
  "set LD_FLAGS=-no-entry -allow-undefined -import-memory\n",
  "set LD_EXPORTS=-export=_wasm_call_ctors -export=main\n",
  "pushd %ProjectPath%\\tests_run_tree\\wasm \n\n",
};
int SetupCommandsLength_WASM = CArrayLength(SetupCommands_WASM);

char* BuildCommands_WASM[] = {
  "clang %TestFileFullPath% %CompilerFlags% -MF %TestFileNoExtensions%.d -o %TestFileNoExtensions%.o\n",
  "wasm-ld %LD_FLAGS% %LD_EXPORTS% %TestFileNoExtensions%.o -o %TestFileNoExtensions%_.wasm\n",
  "wasm-opt --legalize-js-interface -g -O0 %TestFileNoExtensions%_.wasm -o %TestFileNoExtensions%.wasm\n",
};
int BuildCommandsLength_WASM = CArrayLength(BuildCommands_WASM);

char* CleanupCommands_WASM[] = {
  // TODO(PS): run the actual test via a node script
  "popd\n",
};
int CleanupCommandsLength_WASM = CArrayLength(CleanupCommands_WASM);

#endif //SETTINGS_H
