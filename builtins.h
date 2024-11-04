#ifndef BUILTINS_H
#define BUILTINS_H
#include "rush.h" //includes all the necessary libraries

//prototypes, defined in builtins.c
void builtinCD(char** args);

void builtinExit(char** args);

void builtinPath(char** args);

void builtinSleep(char** args);

void redirectOutput(char** args);

void runFuncs(char** args);

#endif