#include "helpers.h"

int needsRedirect(char** args)
{
    int count = 0; //count of redirections
    int i = 0; //index

    while (args[i] != NULL) //traverse the args array 
    {
        if (strcmp(args[i], ">") == 0) //if the current arg is '>', increment count
        {
            count++;

            if (args[i+1] == NULL || args[i+2] != NULL || count > 1) //if there is nothing following the redirection OR if there is more than one redirection OR if there is more than one argument following the redirection --> error
            {
                ERROR();
                return -7;
            }
        } 
        i++; //increment the index
    }

    return (count == 1) ? 1 : 0; //return 1 if there is one redirection, 0 otherwise
}

int isBuiltIn(char** args) //check if the input is a built-in command
{
    if(strcmp(args[0], "cd") == 0 || strcmp(args[0], "exit") == 0 || strcmp(args[0], "path") == 0 || strcmp(args[0], "sleep") == 0)
    {
        return 1;
    }
    return 0;
}

char* trim(char* str) //trim any whitespace from input
{
    char* end; //end of the string

    while(isspace(*str)) //while the current character of the input is whitespace, move to next character
    {
        str++;
    }

    if(*str == 0) //if the input is empty, return the input
    {
        return str;
    }

    end = str + strlen(str) - 1; //set end to the last character of the input (minus 1 to ignore \0)
    while(end > str && (isspace(*end))) //travese the input backwards, omitting white spaces
    {
        end--;
    }

    *(end + 1) = '\0'; //null terminate the input

    return str; //return the trimmed input
}

int hasMultCommands(char* input) 
{
    char *ptr = input; //pointer to the input

    while ((ptr = strchr(ptr, '&')) != NULL) //while there is a multiple command included in the input
    {
        ptr++; //increment the pointer

        while (*ptr == ' ') //ignore any whitespace
        {
            ptr++;
        }

        if (*ptr != '\0') //this means there is a command after the '&'
        {
            return 1;
        }

        else //anything else is invalid
        {
            return 0; 
        }
    }
    return 0; //no more commands 
}

char* normPath(char* args)
{
    char* path = calloc(strlen(args) + 1, sizeof(char)); //allocate and clear memory for the path string, plus one for null terminator

    if (!path) //if input is empty, return NULL
    {
        return NULL; 
    }

    int j=0; //new path index
    int wasSlash = 0; //flag for slash

    for (int i = 0; args[i] != '\0'; i++) 
    {
        if (args[i] == '/') //if the current character is a slash
        {
            if (!wasSlash) //if the previous character was not a slash
            {
                path[j++] = args[i]; //add the slash to the path and increment string index
                wasSlash = 1; //update the flag
            }
        } 
        
        else //if the current character is not a slash, add it to the path, increment index, and reset the flag
        {
            path[j++] = args[i];
            wasSlash = 0;
        }
    }

    path[j] = '\0'; //append null terminator to the path and return the updated path
    return path;
}