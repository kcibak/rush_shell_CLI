#include "parallel.h"

void runParallel(char** commands) 
{
    int pid; //process ID
    int i = 0; //index
    int status; //status of the process
    int numCommands = 0;

    int* pids; //array of process IDs

    while (commands[i] != NULL && i<maxArgs) //while there are still commands to be executed
    {
        char** args = parseInput(commands[i]); //parse the input into args array

        if (args == NULL || args[0] == NULL) //if no commands or no arguments, error, free mem, and return
        {
            ERROR();
            free(args);
            return;
        }

        pid = fork(); //create a new process
        if (pid == 0) //if child process
        {
            if (isBuiltIn(args)) //if the command is a built-in function, run it
            {
                runFuncs(args); 
            } 
            else //otherwise, execute the command
            {
                executeCommand(args);
            }
            free(args); //free the memory allocated for the array of args
            exit(0); //exit the child process
        } 
        
        else if (pid < 0) //if fork fails, print error, free mem, and return
        {
            ERROR();
            free(args);
            return;
        } 
        
        else //otherwise, add the process ID to the array of process IDs, then increment the number of commands
        {
            pids[numCommands++] = pid;
        }

        free(args); //free the memory allocated for the array of args
        i++; //increment the index
    }

    for (i = 0; i < numCommands; i++) //wait for all the processes to finish
    {
        waitpid(pids[i], &status, 0);
    }
}