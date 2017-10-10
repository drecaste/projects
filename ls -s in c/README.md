NAME
	lss - list expanded view on directory contents in decreasing byte size

SYNOPSIS
	lss [OPTION]... [FILE]...
	
DESCRIPTION
	lss displays displays information about the FILEs, if none are supplied, it will list 
	information about the current directory. lss will accept any OPTIONs that "ls" does. 
	If an OPTION is not valid, ls will automatically raise an error. FILEs must be files 
	only, not directories. If a directory is given, information will be given about the 
	directory, but will not be descended.
	
NOTES
	Any type of OPTION will be accepted, even if it changes the sorting output, such as -t.
	The default will sort by decreasing byte size, but if the user wants to change that,
	then they are allowed to. 
