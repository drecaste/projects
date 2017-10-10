#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>

#define PORT 0628
#define MYPORT PORT
#define MAXDATASIZE 300
#define BACKLOG 100

int create_s_fd()
{
	int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if( socket_fd == -1 )
	{
		perror("socket() Error");
		exit(1);
	}
	else
		printf("socket() is OK... \n");

	return socket_fd;
}

void create_sockaddr_in( struct sockaddr_in* my_addr )
{
	memset (my_addr, 0, sizeof(*my_addr));

	my_addr->sin_family = AF_INET;
	my_addr->sin_port = htons(PORT);
	my_addr->sin_addr.s_addr = htonl(INADDR_ANY);
}

void bind_check( int* s_fd, struct sockaddr_in* my_addr )
{
	if( bind( *s_fd, (struct sockaddr*) my_addr, sizeof(*my_addr)) == -1 )
	{
		perror("bind() error");
		exit(1);
	}
	else
		printf("bind() is OK... \n");
	
}

void listen_check( int s_fd )
{
	if( listen(s_fd, BACKLOG) == -1)
	{
		perror("listen() error");
		exit(1);
	}
	else
		printf("listen() is OK...\n");
}

int accept_check( int s_fd, struct sockaddr_in* my_addr)
{
	int sin_size = sizeof(struct sockaddr_in);
	int new_fd = accept( s_fd, (struct sockaddr*)my_addr, (socklen_t*) &sin_size );
	
	if( new_fd == -1 )
		perror("accept() error");
	else
		printf("accept() is OK...\n");

	return new_fd;
}

void server_setup( int* s_fd, struct sockaddr_in* my_addr )
{
	create_sockaddr_in( my_addr );
	bind_check( s_fd, my_addr );
	listen_check( *s_fd );
}

void setup_fd_set( int s_fd, fd_set* active_fd_set )
{
	FD_ZERO( active_fd_set );
	FD_SET( s_fd, active_fd_set );
}

void select_check( fd_set* client_fd_set, fd_set* read_fd_set )
{
	*read_fd_set = *client_fd_set;
	if(select( FD_SETSIZE, read_fd_set, NULL, NULL, NULL) < 0 )
	{
		perror("select() error");
		exit(1);
	}
}

void write_to_all_but_s( int main_fd, int current_fd, char* message, fd_set* client_fd_set )
{
	for(int i = 0; i < FD_SETSIZE; ++i)
	{
		if( FD_ISSET(i, client_fd_set) && i != main_fd && i != current_fd )
		{
			if( write( i, message, strlen(message) ) < strlen(message) )
			{
				printf("Message to %d was not fully written\n", i );
			}
		}
	}
}

void interpret_message( int main_fd, int current_fd, char* message, fd_set* client_fd_set )
{
	write_to_all_but_s( main_fd, current_fd, message, client_fd_set );
	if( strncmp(message, "LEAVE ", 6) == 0 )
	{
		FD_CLR( current_fd, client_fd_set );
		close( current_fd );
	}
}

void run_server( int* s_fd, struct sockaddr_in* my_addr, struct sockaddr_in* client_addr, fd_set* client_fd_set, fd_set* read_fd_set)
{
	while(1)
	{
		select_check( client_fd_set, read_fd_set  );
		
		for(int i = 0; i < FD_SETSIZE; ++i)
		{
			if( FD_ISSET(i, read_fd_set) )
			{
				if( i == *s_fd )
				{
					int new_fd = accept_check( *s_fd, my_addr );
					printf("Connected!\n");
					FD_SET( new_fd, client_fd_set );
				}
				else
				{
					char std_buf[256] = "";
					printf("About to Read\n");
					read( i, std_buf, 256 );
					printf("Message From %d: %s\n", i, std_buf );
					interpret_message( *s_fd, i, std_buf, client_fd_set );
				}
			}
		}
	}
	close( *s_fd );
}

int main()
{
	int s_fd = create_s_fd();
	struct sockaddr_in my_addr, client_addr;
	fd_set client_fd_set, read_fd_set;
	
	server_setup( &s_fd, &my_addr );
	setup_fd_set( s_fd, &client_fd_set);
	run_server( &s_fd, &my_addr, &client_addr, &client_fd_set, &read_fd_set);

	return 0;
}
