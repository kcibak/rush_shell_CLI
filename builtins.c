#include "builtins.h" 
#include "helpers.h" //to use the ERROR() macro

void builtinCD(char** args) 
{
    if (args[1] == NULL || args[2] != NULL) //if there is no argument or more than one argument, error
    {
        ERROR();
    } 
    
    else if (chdir(args[1]) != 0) //if there is one argument, change the directory to the specified path; if chdir fails, error
    {
        ERROR();
    }
}

void builtinExit(char** args) 
{
    if (args[1] != NULL) //if exit is passed with arguments, error
    {
        ERROR();
    } 
    else //if exit is passed properly, exit the shell
    {
        exit(0); 
    }
}

void builtinPath(char** args) 
{
    for (int i = 0; i < maxPaths; i++) //clear the default path
    {
        defaultPath[i] = NULL;
    }

    for (int i = 1; args[i] != NULL && i < maxPaths; i++) //traverse the arguments while they exist and are in range
    {
        char* norm_path = normPath(args[i]); //normalize the path
        if (norm_path != NULL) //while the path is not empty, add it to the default path and free the memory
        {
            defaultPath[i - 1] = strdup(norm_path); //-1 because strdup automatically adds a byte for null terminator and appends null terminator; 
            free(norm_path);
        } 

        else 
        {
            ERROR();
        }
    }
}

void builtinSleep(char** args) 
{
    if (args[1] == NULL || args[2] != NULL) //if there is no time specified or more than one argument, error
    {
        ERROR();
    } 
    
    else 
    {
        int zzz = atoi(args[1]); //convert user-entered time to an integer
        sleep(zzz); //call the sleep function with the specified time
    }
}

void redirectOutput(char** args) 
{
    int redirectIndex = -1; //initialize the index of the redirection; -1 means no redirection

    for (int i = 0; args[i] != NULL; i++) //traverse the arguments array
    {
        if (strcmp(args[i], ">") == 0) //if the current character is redirection, set the index and end loop
        {
            redirectIndex = i;
            break;
        }
    }

    if (redirectIndex == -1 || args[redirectIndex + 1] == NULL) //if there is no redirection or if user did not specify output file, error and return
    {
        ERROR();
        return;
    }

    int out = open(args[redirectIndex + 1], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU); //open the output file, storing it in out
    if (out < 0) //if open fails, error and return
    {
        ERROR();
        return;
    }

    if (dup2(out, STDOUT_FILENO) < 0) //duplicate the output file descriptor to standard output, close, error and return if dup2 fails
    {
        close(out);
        ERROR();
        return;
    }

    close(out); //close the output file if there is no error; out has been written to standard output

    args[redirectIndex] = '\0'; //null terminate the arguments array
}

void runFuncs(char** args) //run the built-in functions
{
    if(strcmp(args[0], "cd") == 0)
    {
        builtinCD(args);
    }
    else if(strcmp(args[0], "exit") == 0)
    {
        builtinExit(args);
    }
    else if(strcmp(args[0], "path") == 0)
    {
        builtinPath(args);
    }
    else if(strcmp(args[0], "sleep") == 0) 
    {
        builtinSleep(args);
    }
}