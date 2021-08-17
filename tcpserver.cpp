#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include <cerrno>
#include <string>
#include <iostream>

// #include "parsing.hpp"
#include "response_builder/response.hpp"


int setup_server()
{
	int 				server_fd;
	int					new_socket;
	struct sockaddr_in	address;
	int					addrlen = sizeof(address);
	const int			PORT = 80;


	/* 1. - Creating the socket*/

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //AFINET = internet socket // SOCK_STREAM = TCP stream
	{
		std::cout << "Failed to create socket. Errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	memset((char *)&address, 0, sizeof(address));
	address.sin_family		= AF_INET;
	address.sin_addr.s_addr	= htonl(INADDR_ANY); // htonl converts a long integer (the address) to a network representation
	address.sin_port		= htons(PORT); // htons converts a short integer (the port) to a network representation

	/* 2. - Naming the socket*/	//basically, say "this socket is going to listen to this address" 

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cout << "Failed to bind. Errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	/* 3. - waiting for an incoming connection*/

	if (listen(server_fd, 3) < 0)
	{
		std::cout << "Failure while listening." << std::endl;
		exit(EXIT_FAILURE);
	}
	return (server_fd);
};

int accept_new_connection(int server_fd, struct sockaddr_in	address, int addrlen)
{
		int	new_socket;

		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
		{
			std::cout << "Failure while accepting connection" << std::endl;
			exit(EXIT_FAILURE);
		}
		return (new_socket);
}

void *handle_connection(int new_socket)
{
	char buffer[1024] = {0};
	long valread;
	int msgsize = 0;
	// char actualpath[PATH_MAX + 1];
	t_http_request		http_req_struct;
	std::string			http_response_firstline;

	while ((valread = read(new_socket, buffer + msgsize, sizeof(buffer) - msgsize - 1)))
	{
		msgsize += valread;
		if (msgsize > 1024 - 1 || buffer[msgsize - 1] == '\n')
			break ;
	}
	buffer[msgsize - 1] = 0;
	std::cout << buffer << std::endl;
	if (valread < 0)
	{
		std::cout << "No bytes are there to read" << std::endl;
	}
		http_request_parser(buffer, http_req_struct);
		print_request_firstline(http_req_struct.firstline);
		print_request_header(http_req_struct.header);
		//the response we want when the client calls the server

		Http_response	test;
		test.Http_response::build_http_response(http_req_struct);
		P("RESPONSE:\n", test.Http_response::get_http_response());
		// P("My Response Server :\t", test.Http_response::get_server());
		// P("My Response Date :\t", test.Http_response::get_date());

		printf("\n------------- Response message sent -------------\n\n");
		fflush(stdout);

		/* 5. - Closing the socket*/
		close(new_socket);
		return (NULL);
}

int main()
{
	fd_set current_sockets;
	fd_set ready_sockets;
	int server_fd;
	
	server_fd = setup_server();
	FD_ZERO(&current_sockets);
	FD_SET(server_fd, &current_sockets);

	/* 4. - communication*/
	while (1) //infinite loop to accept and handle connections
	{
		// because select is destuctive, we make a copy of current_sockets
		ready_sockets = current_sockets;

		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			std::cout << "Failure whit select " << std::endl;
			exit(EXIT_FAILURE);
		}

		std::cout << "\n++++++++ Waiting for new connection ++++++++++\n";
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			// check if this socket i is ready to listen or write
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == server_fd)
				{
					new_socket = accept_new_connection(server_fd, address, addrlen);
					handle_connection(new_socket);
				}
			}
		}
	}

	return 0;
}