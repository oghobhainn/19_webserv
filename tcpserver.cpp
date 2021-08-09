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

#include "parsing.hpp"
#include "response.hpp"

int main()
{

	int 				server_fd;
	int					new_socket;
	long				valread;
	struct sockaddr_in	address;
	int					addrlen = sizeof(address);
	const int			PORT = 8080;

	t_http_request		http_req_struct;
	t_http_response		http_resp_struct;
	std::string			http_response_firstline;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family		= AF_INET;
	address.sin_addr.s_addr	= htonl(INADDR_ANY); // htonl converts a long integer (the address) to a network representation
	address.sin_port		= htons(PORT); // htons converts a short integer (the port) to a network representation


	/* 1. - Creating the socket*/

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //AFINET = internet socket // SOCK_STREAM = TCP stream
	{
		std::cout << "Failed to create socket. Errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

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

	/* 4. - communication*/
	while (1) //infinite loop to accept and handle connections
	{
		printf("\n++++++++ Waiting for new connection ++++++++++\n");

		//new_socket is the fd we've just accepted
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
		{
			std::cout << "Failure while accepting connection" << std::endl;
			exit(EXIT_FAILURE);
		}

		char buffer[1024] = {0};
		valread = read( new_socket, buffer, 4096); //we just read maximum 4096 bytes, not a good way to do but enough for the moment
		std::cout << buffer << std::endl;
		if (valread < 0)
		{
			std::cout << "No bytes are there to read" << std::endl;
		}

		http_request_parser(buffer, http_req_struct);
		print_request_firstline(http_req_struct.firstline);
		print_request_header(http_req_struct.header);
		//the response we want when the client calls the server
		http_response_firstline = (Http_response::build_http_response(http_req_struct)).Http_response::get_firstline();

		int response_length = http_response_firstline.length();
		char response_char[response_length + 1];
		strcpy(response_char, http_response_firstline.c_str());

		write(new_socket, response_char, strlen(response_char));
		
		printf("\n------------- Response message sent -------------\n\n");
		fflush(stdout);
		/* 5. - Closing the socket*/
		close(new_socket);
	}

	return 0;
}
