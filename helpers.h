#ifndef HELPERS_H
#define HELPERS_H
#include "rush.h" //includes all the necessary libraries

//prototypes, defined in helpers.c
int needsRedirect(char** args);

int isBuiltIn(char** args);

char* trim(char* str);

int hasMultCommands(char* input);

char* normPath(char* path);

#endif