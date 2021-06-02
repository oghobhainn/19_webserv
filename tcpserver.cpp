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

int main()
{

	int 				server_fd;
	int					new_socket;
	long				valread;
	struct sockaddr_in	address;
	int					addrlen = sizeof(address);
	const int			PORT = 8080;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY); // htonl converts a long integer (the address) to a network representation
	address.sin_port = htons(PORT); // htons converts a short integer (the port) to a network representation


	/* 1. - Creating the socket*/

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Failed to create socket. Errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	/* 2. - Naming the socket*/

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
	while (1)
	{
		printf("\n++++++++ Waiting for new connection ++++++++++\n");

		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
		{
			std::cout << "Failure while accepting connection" << std::endl;
			exit(EXIT_FAILURE);
		}

		char buffer[1024] = {0};
		valread = read( new_socket, buffer, 1024);
		std::cout << buffer << std::endl;
		if (valread < 0)
		{
			std::cout << "No bytes are there to read" << std::endl;
		}

		//the response we want when the client calls the server
		const char* hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";


		write(new_socket, hello, strlen(hello));
		
		printf("\n------------- Hello message sent -------------\n");

		/* 5. - Closing the socket*/
		close(new_socket);
	}

	return 0;
}
