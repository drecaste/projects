#include "parser.h"

void parse(char * input_command)
{
	// reset the struct
	resetParser();
	
	// get the amount of commands by amount of pipes
    get_amount_of_commands(input_command);
	
	// parse into the commands
	parse_by_spaces(input_command);
	
	// print the command
	//printFunc();
}

struct command returnStruct()
{
	return input;
}

void resetParser()
{
	input = (const struct command){ 0 };
}

void printFunc()
{
    printf("%d: ", input.amount_of_commands);
	
	// print input redirection
	if(strcmp(input.inredir, "") != 0)
	{
	  printf("< '%s' ", input.inredir);
	}
	
	// for every word/command in the struct
    for(int i = 0; i < input.words; i++)
    {
		// if it's a < or > or | do not print the ' quotes ' around it
		// add spacing for readability
        if(strcmp(input.commands[i], ">") == 0 || strcmp(input.commands[i], "<") == 0 || strcmp(input.commands[i], "|") == 0)
        {
            printf("%s ", input.commands[i]);
        }
        else
        {
            printf("'%s' ", input.commands[i]);
        }
    }
	
	// print out redirection
	if(strcmp(input.outredir, "") != 0)
	{
		printf("> '%s'", input.outredir);
	}
	printf("\n");
}

// get the amount of commands in the command input by counting the pipes
void get_amount_of_commands(char * input_command)
{
	// start amount_of_commands at 1
	input.amount_of_commands++;
	
	// until the end of the line
    for(int i = 0; input_command[i] != '\0'; i++)
    {
		// if there is a pipe, there is a command
        if(input_command[i] == '|')
        {
            input.amount_of_commands++;
        }
    }
}

void parse_by_spaces(char * input_command)
{
	char flag;
	// temporary char array and counts
	char temp[1000] = "";
	int currCount = 0;
	
	// while not the end of the string
	for(int i = 0; input_command[i] != '\0'; i++)
	{
		// check if current character is input/output redirection
		if(input_command[i] == '>' || input_command[i] == '<')
		{
			// set the flag stating that there is i/o redirection to the input or output character
			flag = input_command[i];
		}
		
		// if it's a space, it's a new command
		// check if end of line, "temp" before that is a valid command
		else if(input_command[i] == ' ' || input_command[i] == '\0' || input_command[i] == '\n')
		{
			// empty temp values may have been picked up
			// don't want those as commands
			if(strcmp(temp, "") != 0)
			{
				// ansi code for < is 60
				// if input redirection, copy temp value (which is the filename)
				// into the inredir character array
				if(flag == 60)
				{
					strcpy(input.inredir, temp);
					flag = 0;
				}
				// ansi code for < is 62
				// if output redirection, copy temp value (which is the filename)
				// into the outredir character array
				else if(flag == 62)
				{
					strcpy(input.outredir, temp);
					flag = 0;
				}
				
				// no flag
				else
				{
					// copy the temp value into the current struct input into the commands
					// commands is an array of strings
					// increase the amount of words
					strcpy(input.commands[input.words++], temp);
				}
			}

			// reset the temp char array for next command
			memset(&temp[0], 0, sizeof(temp));
			currCount = 0;
		}
		
		// otherwise, add character to temp array/a command or word is being built
		else
		{
			// set the current character/letter equal to the current amount of
			// letters in temp and increase amount of letters
			temp[currCount++] = input_command[i];
		}
	}
}
