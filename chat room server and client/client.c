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

#define DEFAULT_PORT 0628

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

void create_sockaddr_in( uint16_t given_port, uint32_t given_host, struct sockaddr_in* my_addr )
{
	memset(my_addr, 0, sizeof(*my_addr));
	
	my_addr->sin_family = AF_INET;
	my_addr->sin_port = htons(given_port);
	my_addr->sin_addr.s_addr = htonl(given_host);
}

void connect_check(int s_fd, struct sockaddr_in* connect_addr )
{
	if( connect( s_fd, (struct sockaddr*) connect_addr, sizeof(*connect_addr) ) == -1 )
	{
		perror("connect() error");
		exit(1);
	}
	else
		printf("connect is OK...\n");
}

void write_check( int s_fd, char* message, int m_len )
{
	if( write( s_fd, message, m_len ) < m_len )
    {
		printf("Message to %d was not fully written\n",     s_fd );
    }
}

void client_setup( char* user, int s_fd, uint16_t given_port, uint32_t given_host, struct sockaddr_in* my_addr )
{
	char enter_message[256];

	create_sockaddr_in( given_port, given_host, my_addr );
	connect_check( s_fd, my_addr );
	sprintf(enter_message, "ENTER %s\n", user);
	write_check( s_fd, enter_message, strlen(enter_message) );
	printf("%s has joined the chatroom\n", user);
}

void remove_possible_ending_newline( char* str )
{
	int len = strlen(str);
	if( str[len-1] == '\n' )
		str[len-1] = '\0';
}

void respond_to_input(int s_fd, char* message)
{
	remove_possible_ending_newline( message );

	if( strncmp(message, "SAY ", 4 ) == 0 )
		write_check(s_fd, message, strlen(message) );
	else if ( strncmp(message, "LEAVE ", 6) == 0 )
	{
		write_check(s_fd, message, strlen(message) );
		exit(0);
	}
	else
		printf("Invalid Mesage Format\n");
}

void setup_fd_set( fd_set* server_and_stdin, int server_fd)
{
	FD_ZERO( server_and_stdin );
	FD_SET( server_fd, server_and_stdin );
	FD_SET( STDIN_FILENO, server_and_stdin );
}

void select_check( int stdin_fd, fd_set* server_and_stdin, fd_set* read_fd_set  ) 
{
	dup2( stdin_fd, STDIN_FILENO );
	*read_fd_set = *server_and_stdin;
    if(select( FD_SETSIZE, read_fd_set, NULL, NULL, NULL ) < 0 )
    {
        perror("select() error");
        exit(1);
    }
}

void respond_to_stdin( int s_fd )
{
	char input_buf[256] = "";
	fgets(input_buf, 256, stdin );
	respond_to_input( s_fd, input_buf );
}

void interpret_message( char* message )
{
	remove_possible_ending_newline(message);
	if( strncmp(message, "SAY ", 4 ) == 0 )
		printf("%s\n", message+4 );
	else if ( strncmp(message, "LEAVE ", 6) == 0 )
	{
		printf("%s has left the chatroom\n", message+6);
	}
	else if ( strncmp(message, "ENTER ", 6) == 0 )
		printf("%s has entered the chatroom\n", message+6 );
	else
		printf("Someone is incorrectly contacting the server\n");
}

void respond_to_server( int s_fd )
{
	char server_buff[256] = "";
	read( s_fd, server_buff, 256 );
	interpret_message( server_buff );
}

void run_client( int s_fd )
{
	fd_set server_and_stdin, read_fd_set;
	setup_fd_set( &server_and_stdin, s_fd);
	int original_stdin;
	dup2( STDIN_FILENO, original_stdin );
	while( 1 )
	{
		select_check( original_stdin, &server_and_stdin, &read_fd_set );
		
		for( int i = 0; i < FD_SETSIZE; ++i )
		{
			if( FD_ISSET(i, &read_fd_set) )
			{
				if( i == s_fd )
				{
					respond_to_server(s_fd);
				}
				else //stdin
				{
					respond_to_stdin(s_fd);
				}
			}
		}
	}
}

int main( int argc, char* argv[] )
{
	int s_fd = create_s_fd();
	char* user = ( argc >= 2 ? argv[1] : "Default" );
	uint16_t port = ( argc >= 3 ? atoi(argv[2]) : DEFAULT_PORT );
	uint32_t host_name = ( argc == 4 ? atoi(argv[3]) : INADDR_ANY );
	struct sockaddr_in my_addr;	

	client_setup( user, s_fd, port, host_name, &my_addr );
	run_client( s_fd );

	return 0;
}
