SETUP

	1. Create a TRASH directory in the $HOME directory.
	2. Add 'export TRASH="$HOME/TRASH"' to .bashrc file

-----    

SCRIPTS

	NAME
		srm - safe remove a file to a TRASH directory
		
	SYNOPSIS
		srm [FILE]...
		
	DESCRIPTION
		Every FILE that the user inputs will be moved to the temporary TRASH. If there is a FILE already
		called FILE in the TRASH, ".bak~N~: will be appended to the end of the FILE, where N is the amount
		of files already called FILE. If the FILE does not exist, an error will occur. If srm is called
		with an entire directory, the entire directory hierarchy will be safe-removed.

-----    
    
	NAME
		unrm - un safe remove a file from the TRASH directory
		
	SYNOPSIS
		unrm [FILE]...
		
	DESCRIPTION
		Every FILE that the user inputs will be removed from the temporary TRASH and moved to the current
		directory. FILE can also be a directory, if it is a directory, the entire directory and its contents
		will be moved to the current directory. If the FILE does not exist in the TRASH, an error will occur.

	OPTIONS
		-a = unremove all of the files in the $TRASH to the current working directory

-----    
	
	NAME
		lsrm - view all safe removed files
		
	DESCRIPTION
		View lsrm will perform a "ls -lR" command on all of the files in the TRASH directory.
		
	OPTIONS
		lsrm does not take any options.

-----    
		
	NAME
		durm - view the disk usage of all safe removed files

	DESCRIPTION
		Summarize the disk usage of every file in the TRASH directory recursively.

	OPTIONS
		durm does not take any options.

-----    
		
	NAME
		trash - delete all safe removed files
		
	DESCRIPTION
		Performs a "rm" command on every file in the TRASH directory, this will permanently
		delete every file.
		
	OPTIONS
		trash does not take any options.
