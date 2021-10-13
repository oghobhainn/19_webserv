// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>

#define PORT 85

std::string readFileIntoString(const std::string& path) {
	std::ifstream input_file(path);
    if (!input_file.is_open()) {
		std::cerr << "Could not open the file - '" << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

int main(int argc, char const *argv[])
{
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;

	if (argc != 2)
	{
		printf("wrong number of arguments, plz add a request_example");
		return 1;
	}
   // char hello[1024] = "GET /index.html HTTP/1.1\r\nHost: 192.241.213.65:6880\r\nContent-Type: text/plain\r\nContent-Length: 27\r\n\r\nDeep down the rabbit hole,\nThere lies him, the CLIENT\r\n";
   	
	std::string filename(argv[1]);
	std::string file_contents;

	file_contents = readFileIntoString(filename);
	char hello[file_contents.length()];
	strcpy(hello, file_contents.c_str());
	char buffer[1024] = {0};
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Socket is [%d]\n", sock);
    printf("message is [\n%s]\n", hello);
    printf("-- Hello message sent --\n");
    printf("Answer from server : \n");
    fflush(stdout);
    valread = read( sock , buffer, 4096);
    printf("\nbegin : \n\n%s\ndone",buffer );
    fflush(stdout);
    return 0;
}
