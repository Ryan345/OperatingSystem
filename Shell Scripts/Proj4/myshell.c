// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:______________Ryan Wake_______________ Date:___04/14/16___

// 3460:426 Lab 4A - Basic C shell

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_ARGS	64
#define MAX_ARG_LEN	16
#define MAX_LINE_LEN	80
#define WHITESPACE	" ,\t\n"
#define TRUE            1

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
void convertCommand(struct command_t *, int *);
void printHelp();

int main(int argc, char *argv[]) {
   int pid;
   int back = 0;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;

   //...
   /* Shell initialization */
   //...

   while (TRUE) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
     // ...
      parseCommand(cmdLine, &command);
     // ...
      convertCommand(&command,&back);
      command.argv[command.argc] = NULL;

      /* Create a child process to execute the command */
      if ((pid = fork()) == 0) {
         /* Child executing command */
         execvp(command.name, command.argv);
      }
      if (back == 0)
      {
         /* Wait for the child to terminate */
         wait(&status);
      }
      else
      {
         waitpid(-1,&status,WNOHANG);
      }
   }

   /* Shell termination */
   printf("\n\n shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);
   return 1;
}

/* End parseCommand function */

void convertCommand(struct command_t *cmd, int *back)
{
   // Compare the command that was typed, to see if we can convert it
   if (strcmp(cmd->name,"C") == 0)
   {
      // Free the memory that was allocated before.
      free(cmd->name);
      // Allocate memory for the new command.
      cmd->name = (char *) malloc(sizeof("cp"));
      // Copy new command to the name.
      strcpy(cmd->name, "cp");
   }
   else if (strcmp(cmd->name,"D") == 0)
   {
      free(cmd->name);
      cmd->name = (char *) malloc(sizeof("rm"));
      strcpy(cmd->name, "rm");
   }
   else if (strcmp(cmd->name,"M") == 0)
   {
       free(cmd->name);
       cmd->name = (char *) malloc(sizeof("nano"));
       strcpy(cmd->name, "nano");
   }
   else if (strcmp(cmd->name,"H") == 0)
   {
      free(cmd->name);
      cmd->name = (char *) malloc(sizeof(""));
      strcpy(cmd->name, "");
      printHelp();
   }
   else if (strcmp(cmd->name,"P") == 0)
   {
      free(cmd->name);
      cmd->name = (char *) malloc(sizeof("more"));
      strcpy(cmd->name, "more");
   }
   else if (strcmp(cmd->name,"S") == 0)
   {
      *back = 1;
      free(cmd->name);
      cmd->name = (char *) malloc(sizeof("firefox"));
      strcpy(cmd->name, "firefox");
   }
   else if (strcmp(cmd->name,"W") == 0)
   {
      free(cmd->name);
      cmd->name = (char *) malloc(sizeof("clear"));
      strcpy(cmd->name, "clear");
   }
   else if (strcmp(cmd->name,"E") == 0)
   {
      free(cmd->name);
      cmd->name = (char *) malloc(sizeof("echo"));
      strcpy(cmd->name, "echo");
   }
   else if (strcmp(cmd->name,"X") == 0)
   {
      free(cmd->name);
      cmd->name = (char *) malloc(sizeof(cmd->argv[1]));
      // Copy argv[1] (filename to execute) to the cmd->name so it will just execute the passed file.
      strcpy(cmd->name, cmd->argv[1]);
   }
   else if (strcmp(cmd->name,"L") == 0)
   {
      free(cmd->name);
      // Print a new line.
      printf("\n");
      cmd->name = (char *) malloc(sizeof("pwd"));
      strcpy(cmd->name, "pwd");
   }
   else if (strcmp(cmd->name,"Q") == 0)
   {
      free(cmd->name);
      // Exit the shell, let parent know it exited successfully
      exit(EXIT_SUCCESS);
   }
}

void printHelp()
{
   printf("\n\n");
   printf("Command:                     Usage:\n");
   printf("C <file1> <file2>            Copy file1 to a new file2.\n");
   printf("D <file>                     Delete file.\n");
   printf("E <text>                     Prints specified text to console.\n");
   printf("H                            Prints this help.\n");
   printf("L                            Shows current working directory, and contents of current directory.\n");
   printf("M <file>                     Opens editor to edit or create a file.\n");
   printf("P <file>                     Displays contents of file.\n");
   printf("Q                            Terminates execution of shell.\n");
   printf("S                            Launches firefox as background process.\n");
   printf("W                            Clears the screen.\n");
   printf("X <program>                  Executes specified file.\n");
}

/* Print prompt and read command functions - pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   char* promptString = "linux (raw93)|>";
   printf("%s ", promptString);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, MAX_LINE_LEN, stdin);
}

/* End printPrompt and readCommand */
