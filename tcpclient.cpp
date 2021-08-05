// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    //char *hello = "Hello from client";
    char *hello = "GET info.html HTTP 1.1\r\nHost: 192.241.213.65:6880\r\nContent-Type: text/plain\r\nContent-Length: 27\r\n\r\nHello world! This is me, CLIENT";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    //if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    //if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)

    // //       int inet_pton(int af, const char *restrict src, void *restrict dst);
    // //This function converts the character string src into a network
    //    address structure in the af address family, then copies the
    //    network address structure to dst.
    // //src points to a character string containing an IPv4
    //           network address in dotted-decimal format,
    //           "ddd.ddd.ddd.ddd", where ddd is a decimal number of up to
    //           three digits in the range 0 to 255.

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
    printf("-- Hello message sent --\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
