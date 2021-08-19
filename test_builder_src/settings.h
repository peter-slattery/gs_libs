/* date = May 10th 2021 2:47 pm */

#ifndef SETTINGS_H
#define SETTINGS_H

char* SetupCommands[] = {
    "@echo off\n",
    "IF NOT EXIST C:\\projects\\gs_libs\\tests_run_tree mkdir C:\\projects\\gs_libs\\tests_run_tree\n",
    "set CompilerFlags=-O0 -g\n",
    "set LinkerLibraries=user32.lib gdi32.lib msvcrt.lib msvcprt.lib opengl32.lib\n",
    "pushd C:\\projects\\gs_libs\\tests_run_tree \n\n",
};

// for each test, you have access to
//   TestFileFullPath
//   TestFileNoExtensions
char* BuildCommand[] = {
    "clang %CompilerFlags% -c %TestFileFullPath%\n",
    "link /NOLOGO -debug %TestFileNoExtensions%.o %LinkerLibraries%\n",
    "%TestFileNoExtensions%.exe\n",
};

char* CleanupCommands[] = {
    "popd\n",
};

#endif //SETTINGS_H
