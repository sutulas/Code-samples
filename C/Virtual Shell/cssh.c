/*#define _POSIX_C_SOURCE 200809L // required for strdup() on cslab
#define _DEFAULT_SOURCE // required for strsep() on cslab
#define _BSD_SOURCE // required for strsep() on cslab*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define MAX_ARGS 32

char **get_next_command(size_t *num_args)
{
    // print the prompt
    printf("cssh$ ");

    // get the next line of input
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);
    if (ferror(stdin))
    {
        perror("getline");
        exit(1);
    }
    if (feof(stdin))
    {
        return NULL;
    }

    // turn the line into an array of words
    char **words = (char **)malloc(MAX_ARGS*sizeof(char *));
    int i=0;

    char *parse = line;
    while (parse != NULL)
    {
        char *word = strsep(&parse, " \t\r\f\n");
        if (strlen(word) != 0)
        {
            words[i++] = strdup(word);
        }
    }
    *num_args = i;
    for (; i<MAX_ARGS; ++i)
    {
        words[i] = NULL;
    }

    // all the words are in the array now, so free the original line
    free(line);

    return words;
}

void free_command(char **words)
{
    for (int i=0; i<MAX_ARGS; ++i)
    {
        if (words[i] == NULL)
        {
            break;
        }
        free(words[i]);
    }
    free(words);
}

void run(char **command, char *infilename, char *outfilename, int append_check)
{
    if (command == NULL || infilename == NULL || outfilename == NULL)
    {
        printf("Error! NULL error!\n");
        return;
    }
    
    if(strcmp(command[0],"exit") == 0) 
    {
        exit(1);
    }
    pid_t fork_rv = fork();
    if (fork_rv == -1)
    {
        perror("fork");
        exit(1);
    }
    
    if (fork_rv == 0)
    {
         // in the child process
        if(strcmp(infilename, "STDIN_FILENO") != 0)
        {
            int read_fd = open(infilename, O_RDONLY);
            if (read_fd == -1)
            {
                perror("open");
                exit(1);
            }
 
          // dup read_fd onto stdin
            if (dup2(read_fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                exit(1);
            }
            close(read_fd);
        }
        if(strcmp(outfilename, "STDOUT_FILENO") != 0)
        {
            int write_fd;
            if (append_check == 1)
            {
                write_fd = open(outfilename, O_WRONLY | O_APPEND | O_CREAT, 0644);
            }
            else
            {
                write_fd = open(outfilename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            }
            if (write_fd == -1)
            {
                perror("open");
                exit(1);
            }
 
          // dup write_fd onto stdout
            if (dup2(write_fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(1);
            }
            close(write_fd);
        }
        
        execvp(command[0], command);
 
        // will never get here if things work correctly
        perror("execvp");
        exit(1);
      }
      if (waitpid(fork_rv, NULL, 0) == -1)
      {
          perror("waitpid");
          exit(1);
      }   
}

int check_help(int counter, char *next)
{

    if(counter < 1)
    {
        printf("Error! Not enough arguments for >!\n");
        return 1;           
            
    }
    if (next == NULL)
    {
        printf("Error! Invalid file descriptor!\n");
        return 1;
    }
    return 0;
}
int iocheck(int counter,char *current, char *next, char *infile, char *outfile)
{
    
    if(strcmp(current, ">") == 0)
    {
        if (check_help(counter, next) == 1)
        {
            return 1;
        }
        if (strcmp(outfile, "STDOUT_FILENO") == 0 && strcmp(current, ">") == 0)
        {
            return 0;           
        }
        if (strcmp(outfile, "STDOUT_FILENO") != 0)
        {
            printf("Error! Can't have two >'s or >>'s!\n");
            return 1;
        }
    }
    if(strcmp(current, ">>") == 0)
    {
        if (check_help(counter, next) == 1)
        {
            return 1;
        }
        if (strcmp(outfile, "STDOUT_FILENO") == 0 && strcmp(current, ">>") == 0)
        {
            return 6;           
        }
        if (strcmp(outfile, "STDOUT_FILENO") != 0)
        {
            printf("Error! Can't have two >'s or >>'s!\n");
            return 1;
        }
    }
    if(strcmp(current, "<") == 0)
    {
        if (check_help(counter, next) == 1)
        {
            return 1;
        }
        if (strcmp(infile, "STDIN_FILENO") == 0)
        {
            return 2;           
        }
        if (strcmp(infile, "STDIN_FILENO") != 0)
        {
            printf("Error! Can't have two <'s!\n");
            return 1;
        }
    }
   return 3;
}
int main()
{
    size_t num_args;
    
    // get the next command
    char **command_line_words = get_next_command(&num_args);
    while (command_line_words != NULL)
    {
        // run the command here
        // don't forget to skip blank commands
        // and add something to stop the loop if the user 
        // runs "exit"
        int counter = 0;
        char *infile = "STDIN_FILENO";
        char *outfile = "STDOUT_FILENO";
        int flag = 3;  
        int append_check =0;
        while (command_line_words[counter] != NULL && flag != 1)
        {
            
            flag = iocheck(counter,command_line_words[counter], command_line_words[counter+1], infile, outfile);
            if (flag == 0)
            {
                outfile = command_line_words[counter+1];
                command_line_words[counter] = NULL;
            }
            if (flag == 2)
            {
                infile = command_line_words[counter + 1];
                command_line_words[counter] = NULL;
            }
            if (flag == 6)
            {
                outfile = command_line_words[counter +1];
                command_line_words[counter] = NULL;
                append_check =1;
            }
            counter ++;
        }
        if (command_line_words[0] == NULL || strcmp(command_line_words[0], " ") ==0|| strcmp(command_line_words[0], "") == 0)
        {
            flag = 1;
        }
        if (flag !=1)
        {
            run(command_line_words, infile, outfile, append_check);        
        }
        // free the memory for this command
        free_command(command_line_words);

        // get the next command
        command_line_words = get_next_command(&num_args);
    }

    // free the memory for the last command
    free_command(command_line_words);

    return 0;
}
