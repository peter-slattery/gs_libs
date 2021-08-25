/* date = May 10th 2021 2:47 pm */

#ifndef SETTINGS_H
#define SETTINGS_H

// Win32

char* SetupCommands_Win32[] = {
  "@echo off\n",
  "SET MyPath=%~dp0\n",
  "SET BinPath=%MyPath:~0,-1%\n",
  "SET ProjectPath=%BinPath%\\.. \n",
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

#endif //SETTINGS_H
