# gs_libs
Libraries for making bare-bones C++ easier to use and prototype with.

## Goals
Create a suite of functionality that:
1. Makes programming in C++ more performant by default
2. Is easy to get up and running with quickly
3. Is easy to debug and modify to the needs of each project
4. Enables cross platform development

## A Note On Single Header Libraries
I love stb style libraries, however, I find that using extensive preprocessor directives to control what parts of the file is included to be pretty illegible.
I generally use these by just including the ones I need at the beginning of my program. Since they don't depend on eachother (other than gs_types), order doesn't matter so this is pretty painless. I'm looking into other options (compiling all into one file, including a gs_libs.h that's just a bunch of includes, etc.) but at the moment, I haven't gotten to it yet.

## Usage
Each file is a suite of functionality by itself and can be used independently of every other library in the set. The exception is gs_types.h and gs_types.cpp. Every library relies on these two files, and they must be included before any others.