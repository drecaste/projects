#include "parser.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int doCommand();
void SIGQUITHandler(int signum);
void testScripts(int argc, char * argv[]);

int main(int argc, char * argv[])
{
	if(argc > 1)
	{
		testScripts(argc, argv);
	}
	
	char input_cmd[10000] = "";

	// print prompt
	printf("? ");
	
	// ignore ctrl-c (use ctrl-\ to quit)
	signal(SIGINT, SIG_IGN);

	// exit with exit-code (ctrl-\)
	signal(SIGQUIT, SIGQUITHandler);
	
	// continuously get input from user
	while(fgets(input_cmd, sizeof(input_cmd), stdin))
	{
		// parse / print input
		parse(input_cmd);
		
		// once the input is parsed, execute the command
		doCommand();
		
		// print prompt for next loop
		printf("? ");
	}

	return 0;
}

void testScripts(int argc, char * argv[])
{
	for(int i = 1; i < argc; i++)
	{
		FILE * fp = fopen(argv[i], "r");
		char line[10000];
		while(fgets(line, sizeof(line), fp))
		{
			// printf("%s", line);
			parse(line);
			doCommand();
		}
		fclose(fp);
	}
}

int doCommand()
{
	// take care of comments
	if(input.commands[0][0] == '#')
	{
		return 0;
	}
	
	// change directory
	//char buf[1000];
	if(strcmp(input.commands[0], "cd") == 0)
	{
		//printf("Current directory: %s\n", getcwd(buf, sizeof(buf)));
		chdir(input.commands[1]);
		//printf("Current directory is now: %s\n", getcwd(buf, sizeof(buf)));
	}
	
	// handle rest of commands
	char * firstargv[1000];
	char * secargv[1000];
	int firstcount = 0;
	int seccount = 0;
	int firstcommand = 0;
	// go through all strings, make sure they're not pipes or input output
	for(int i = 0; i < input.words; i++)
	{
		if(strcmp(input.commands[i], ">") != 0 && strcmp(input.commands[i], "<") != 0)
		{
			// if there is a pipe, we are no longer in the first command
			// next input.commands need to go into the secargv, to be exectued
			if(strcmp(input.commands[i], "|") == 0)
			{
				firstcommand = 1;
			}
			else
			{
				if(firstcommand == 0)
				{
					firstargv[firstcount++] = input.commands[i];
				}
				else
				{
					secargv[seccount++] = input.commands[i];
				}
			}
		}
	}

	// set last equal to NULL in each first and second argv
	firstargv[firstcount++] = NULL;
	secargv[seccount++] = NULL;

	
	// fork and execute command
	// wait for child to finish executing command
	int status;
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("First fork() failed");
		return -1;
	}
	
	// if there's only one command to run, run it
	if(input.amount_of_commands == 1)
	{
		if(pid == 0)
		{
			// input file
			if(strcmp(input.inredir, "") != 0)
			{
				int fdin;
				if((fdin = open(input.inredir, O_RDONLY, 0)) < 0)
				{
					perror("Error: input file");
					exit(0);
				}
				dup2(fdin, STDIN_FILENO); //0
				close(fdin);
			}
			
			// output file
			if(strcmp(input.outredir, "") != 0)
			{
				int fdout;
				if((fdout = creat(input.outredir, 0644)) < 0)
				{
					perror("Error: output file");
					exit(0);
				}
				dup2(fdout, STDOUT_FILENO); // 1
				close(fdout);
			}
			
			// execute command
			if(execvp(firstargv[0], firstargv) == -1)
			{
				perror("Error");
				exit(1);
			}
		}
		else
		{	// wait for child to finish
			waitpid(pid, &status, 0);
		}
	}
	
	
	
	
	// otherwise, there are pipes
	else
	{
		int pipes[2];
		if(pipe(pipes) == -1)
		{
			perror("Pipes failed");
			exit(1);
		}
		
		// first command
		pid_t pid1;
		pid1 = fork();
		if (pid1 < 0)
		{
			perror("First fork() failed");
			return -1;
		}
		if(pid1 == 0)
		{
			close(1);
			dup(pipes[1]);
			close(pipes[0]);
			close(pipes[1]);
			if(execvp(firstargv[0], firstargv) == -1)
			{
				perror("Error");
				exit(1);
			}
		}
		
		// second command
		pid_t pid2 = fork();
		if(pid2 < 0)
		{
			perror("Second fork() failed");
			return -1;
		}
		if(pid2 == 0)
		{

			close(0);
			dup(pipes[0]);
			close(pipes[0]);
			close(pipes[1]);
			if(execvp(secargv[0], secargv) == -1)
			{
				perror("Error");
				exit(1);
			}
		}
		close(pipes[0]);
		close(pipes[1]);
		waitpid(pid1, &status, 0);
		waitpid(pid2, &status, 0);
	}
	return 0;
}

void SIGQUITHandler(int sig_num)
{
    signal(SIGQUIT, SIGQUITHandler);
    printf("\n");
    fflush(stdout);
	exit(0);
}