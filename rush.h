/*
Kira Cibak
U06950566
kcibak@usf.edu
This program consists of 4 source files and 4 header files. There is also a Makefile used to compile and run the executable. This program is a command line interpreter. It allows a user to type commands in response to the prompt '>rush'. The shell then creates processes and executes the user-specified command(s) when the child process has finished.
*/


#ifndef RUSH_H
#define RUSH_H

#include <stdio.h> //to use printf(), getline(), and fflush()
#include <stdlib.h> //to use exit()
#include <unistd.h> //to use fork(), execv(), and the other built-ins
#include <string.h> //to use strtok(), strcmp(), and strdup()
#include <ctype.h> //to use isspace()
#include <sys/wait.h> //to use waitpid()
#include <fcntl.h> //to use open()

#define maxPaths 7
#define maxArgs 7
#define maxIn 255 //max number of characters in input

extern char* defaultPath[maxPaths]; //default path declaration, defined in rush.c

//prototypes, defined in rush.c

void runShell(int num);

char** parseInput(char* input);

void executeCommand(char** args);

void ERROR();

#endif