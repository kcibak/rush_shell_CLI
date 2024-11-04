#include "rush.h"
#include "builtins.h"
#include "helpers.h"
#include "parallel.h"

char* defaultPath[maxPaths] = {"/bin", NULL}; //default path definition

int main(int argc) //main function of the rush CLI
{
    runShell(argc); //run the rush shell
}

void runShell(int num)
{
    if(num > 1) //if rush is run with arguments, print error and exit
    {
        ERROR();
        exit(1);
    }

    char *inputLine = NULL; //string to hold user input
    int len = 0; //length of input
    int read = 0; //number of characters read from input

    while (1) //infinite rush loop
    {
        printf("rush> ");
        fflush(stdout); //force buffered data to be written to the terminal immediately

        if((read = getline(&inputLine, &len, stdin)) == -1) //get input from user, error if input is NULL
        {
            ERROR();
        }

        if (inputLine[read - 1] == '\n') //append null terminator to input, replacing newline
        {
            inputLine[read - 1] = '\0';
        }

        inputLine = trim(inputLine); //trim any extraneous whitespace from the input

        if(strlen(inputLine) == 0 || strlen(inputLine) == 1) //if input is empty or only contains a newline, continue
        {
            continue;
        }

        if (hasMultCommands(inputLine)) //if input contains multiple args, run them in parallel
        {
            char* multArgs[maxArgs]; //array to hold multiple args
            char* command = strtok(inputLine, "&"); //splits the input into separate args, delimited by '&'
            int idx = 0; //index of the first command

            while (command != NULL) //while there are still args to be run
            {
                multArgs[idx++] = trim(command); //trim whitespace from the command and add it to the array, then increment index
                command = strtok(NULL, "&"); //continue splitting the input into separate args, becomes NULL when no more args
            }

            multArgs[idx] = NULL; //append null terminator to the array of args
            runParallel(multArgs);
        } 
        
        else //if input contains only one command
        {
            char** args = parseInput(inputLine);

            if (args == NULL) //if input is NULL, print error and continue
            {
                ERROR();
                continue;
            }

            if (isBuiltIn(args)) //if input is a built-in command, run it
            {
                runFuncs(args);
            } 

            else //if input is not a built-in command, execute it
            {
                executeCommand(args);
            }
            free(args); //free the memory allocated for the array of parsed input
        }
    }
    return;
}

void ERROR() //function to print an error message
{
    char* message = "An error has occurred\n";
    write(STDERR_FILENO, message, strlen(message)); //write the error message to the terminal
    fflush(stderr); //force buffered data to be written to the terminal immediately
}

char** parseInput(char* input)
{
    int max = maxArgs; //max number of arguments, used for calloc
    int idx = 0; //index of the first argument
    char** args = malloc(max * sizeof(char*)); //array to hold the parsed input

    if (!args) //if memory allocation fails, print error and return NULL
    {
        ERROR();
        return NULL;
    }

    char* currCommand = strtok(input, " \n"); //split the input into separate commands, delimited by whitespace or newline

    while (currCommand != NULL) //while there are still commands to be parsed
    {
        args[idx++] = currCommand; //add the current command to the array of args and increment the index
        currCommand = strtok(NULL, " \n"); //continue splitting the input into separate commands, becomes NULL when no more commands
    }
    args[idx] = NULL; //append null terminator to the array of args

    if (idx == 0) //if no commands, free the memory and return NULL
    {
        return NULL;
    }
    return args; //return the array of parsed input
}

void executeCommand(char** args) 
{
    int pid; //process ID
    int status; //status of the process

    int check = needsRedirect(args); //check if the input involves redirection
    if (check == -7) //if isRediret returns -1, return (error will print in isRedirect)
    {
        return;
    }

    pid = fork(); //create a new process
    if (pid == 0) //if child process
    {
        if (check == 1) //if input involves redirection, call redirectOutput
        {
            redirectOutput(args);
        }

        for (int i = 0; defaultPath[i] != NULL; i++) //traverse the default path
        {
            char command[maxIn]; //command to be executed
            snprintf(command, sizeof(command), "%s/%s", defaultPath[i], args[0]); //format the command

            execv(command, args); //execute the command wiht the arguments
        }

        ERROR(); //if execv fails, print error and exit
        exit(1);

    } 
    
    else if (pid < 0) //if fork fails, print error
    {
        ERROR();
    } 
    
    else 
    {
        waitpid(pid, &status, 0); //wait for the child process to finish
        fflush(stdout); //force buffered data to be written to the terminal immediately
    }
}