#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct command input;

struct command
{
	// amount of commands separated by pipes
    int amount_of_commands;
	
	// amount of words in the command
	// used to print
    int words;
	
	// array of strings used to store commands/words
    char commands[10000][10000];
	
	// if there is input redirection
	// file name will be stored here
	char inredir[100];

	// if there is output redirection
	// file name will be stored here
	char outredir[100];
};

// print the entire struct
void printFunc();

// parse the input
void parse(char * input);

// get the amount of commands by counting pipes
void get_amount_of_commands(char * input_command);

// parse the input_command by spaces to get the individual commands
void parse_by_spaces(char * input_command);

// reset all items in struct to empty/default
void resetParser();

struct command returnStruct();
