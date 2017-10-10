#include "parser.h"

int main()
{
	char input_cmd[10000] = "";

	// print prompt
	printf("? ");
	
	// continuously get input from user
	while(fgets(input_cmd, sizeof(input_cmd), stdin))
	{
		// parse / print input
		parse(input_cmd);
		
		// print prompt for next loop
		printf("? ");
	}

	return 0;
}