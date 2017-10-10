// COMPILE WITH: 	gcc -std=c99 lss.c -o lss
// EXECUTE: 		lss [args] [files]

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>  
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include<stdlib.h>
#include<string.h>

// stat structure to contain all files/dirs needed for ls print
struct all_stat
{
	char perm[15];
	int links;
	char * owner;
	char * group;
	int size;
	char date[100];
	char * name;
}	all_stats[10000];
// array of 10000 
// (INT_MAX would be preferable - there can be many files in a directory but it takes too long)


// global variables
int printed = 0;
int currstats = 0;
int currInt = 0;
int a_opt = 0;
int A_opt = 0;
int L_opt = 0;
int onlyargs = 0;

char pathName[1000];
char currdir[1000];
struct dirent *ent;
struct stat ss;
DIR *dDIR;

// functions
int compare(const void * a, const void * b);
void get_stat(struct stat ss, char * name);
void display();
void concat(char * temp, char * path);
void go_through_dir(char * dir);
void check_opts(char * argument);
void do_sort();


int main(int argc, char * argv[])
{
	// get the current working directory
	getcwd(currdir, sizeof(currdir));
	
	// if nothing is passed as an argument, display files for cwd
	if(argc == 1)
	{
		go_through_dir(currdir);
	}
	
	// options were passed
	else
	{
		for(int i = 1; i < argc; i++)
		{
			stat(argv[i], &ss);
			// if it is a file, add to struct array
			if(S_ISREG(ss.st_mode))
			{
				get_stat(ss, argv[i]);
			}
			// if it is a dir, go through dir and add to struct array
			else if(S_ISDIR(ss.st_mode))
			{
				go_through_dir(argv[i]);
			}
			// check if there are any arguments
			else if(argv[i][0] == '-')
			{
				check_opts(argv[i]);
				onlyargs++;
			}
			// not a file/dir or arg -> print error
			else
			{
				printf("Error - '%s' does not exist\n", argv[i]);
			}
			
			// if only arguments were passed, and no files/dirs
			// go through current directory
			if(onlyargs == argc - 1)
			{
				go_through_dir(currdir);
			}
		}
		do_sort();
	}
	return 0;
}

// concatenation for paths
void concat(char * temp, char * path)
{
	// copy the path name into the temp variable
	strcpy(temp, path);
	size_t len = strlen(temp);
	
	// add forward slash if needed
	if(temp[len - 1] != '/')
		strcat(temp, "/");
}


// check if any options were given in the command line
void check_opts(char * argument)
{
	for(int i = 1; i < strlen(argument); i++)
	{
		switch(argument[i])
		{
			case 'A':
				A_opt = 1;
				break;
			// 'a' is default behaviour - specified in a4 assignment
			case 'a':
				a_opt = 1;
				break;
			case 'L':
				L_opt = 1;
				break;
		}
	}
}

// go through a directory, get all files, then sort
void go_through_dir(char * dir)
{
	dDIR = opendir(dir);
	
	// while there are files to go through
	while((ent = readdir(dDIR)) != NULL)
	{
		// get the entire path name of the file
		concat(pathName, currdir);
		strcat(pathName, ent->d_name);
		
		stat(pathName, &ss);

		// if it is a regular file or directory, print out the information
		if(S_ISREG(ss.st_mode) || S_ISDIR(ss.st_mode))
		{
			// if the A option was specified, hide "." and ".." files
			if(A_opt == 1)
			{
				// make sure file does not start with "." or ".."
				if(strcmp(ent->d_name, ".") != 0 &&  strcmp(ent->d_name, "..") != 0)
				{
					get_stat(ss, ent->d_name);
				}
			}
			// do regular check without A_opt
			else
			{
				get_stat(ss, ent->d_name);
			}
		}
		// reset the path, for next file/dir
		strcpy(pathName, currdir);
	}
	do_sort();
}

// get the ls "-l" information of a file and store into array of all_stat structs
void get_stat(struct stat ss, char * name)
{
	// time
	char timeb[200];
	struct tm *tm = localtime(&ss.st_mtime);
	strftime(all_stats[currstats].date, sizeof(all_stats[currstats].date), "%b %d %H:%M ", tm);
	
	
	// permissions
	all_stats[currstats].perm[0] = (S_ISDIR(ss.st_mode)) ? 'd' : '-';
	all_stats[currstats].perm[1] = (ss.st_mode & S_IRUSR) ? 'r' : '-';
	all_stats[currstats].perm[2] = (ss.st_mode & S_IWUSR) ? 'w' : '-';
	all_stats[currstats].perm[3] = (ss.st_mode & S_IXUSR) ? 'x' : '-';
	all_stats[currstats].perm[4] = (ss.st_mode & S_IRGRP) ? 'r' : '-';
	all_stats[currstats].perm[5] = (ss.st_mode & S_IWGRP) ? 'w' : '-';
	all_stats[currstats].perm[6] = (ss.st_mode & S_IXGRP) ? 'x' : '-';
	all_stats[currstats].perm[7] = (ss.st_mode & S_IROTH) ? 'r' : '-';
	all_stats[currstats].perm[8] = (ss.st_mode & S_IWOTH) ? 'w' : '-';
	all_stats[currstats].perm[9] = (ss.st_mode & S_IXOTH) ? 'x' : '-';
	
	// links, owner, group, size, name
	all_stats[currstats].links = ss.st_nlink;
	all_stats[currstats].owner = getpwuid(ss.st_uid)->pw_name;
	all_stats[currstats].group = getgrgid(ss.st_gid)->gr_name;
	all_stats[currstats].size = ss.st_size;
	all_stats[currstats].name = name;
	
	currstats++;
}


// compare function
int compare(const void * a, const void * b) 
{
	// b - a instead of a - b for reverse size order (-S option for ls)
     return ((struct all_stat *) b )->size - ((struct all_stat * ) a )->size;
}

// sort the array by size
void do_sort()
{
	qsort(all_stats, 10000, sizeof(struct all_stat), compare);  

	// once done sorting, display all of the file information
	display();
}


// print the information in "ls -l" format
void display()
{	
	// check if all_stats has already been printed, if not, print
	if(printed == 0)
	{
		for(int i = 0; i < currstats; i++)
		{
			// permissions
			printf("%s", all_stats[i].perm);
			
			// amount of links
			printf(" %d", all_stats[i].links);
			
			// user, group, size, time, name
			printf(" %-8.8s %-5.8s %7jd %-9s", all_stats[i].owner, all_stats[i].group, all_stats[i].size, all_stats[i].date);
			printf("%s\n", all_stats[i].name);
		}
		printed = 1;
	}
}