#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include <string>

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
		perror("cannot create socket"); //not auth function
		return (0);
	}

	/* 2. - Naming the socket*/

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		return (0);
	}

	/* 3. - waiting for an incoming connection*/

	if (listen(server_fd, 3) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	/* 4. - communication*/
	while (1)
	{
		printf("\n++++++++ Waiting for new connection ++++++++++\n");

		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}

		char buffer[1024] = {0};
		valread = read( new_socket, buffer, 1024);
		printf("%s\n", buffer);
		if (valread < 0)
		{
			printf("No bytes are there to read");
		}

		//the response we want when the client calls the server
		std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

		write(new_socket, hello, strlen(hello));
		
		printf("\n------------- Hello message sent -------------\n");

		/* 5. - Closing the socket*/
		close(new_socket);
	}

	return 0;
}
