#ifndef SimpleSocket_hpp
#define SimpleSocket_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>


class SimpleSocket
{
    private:
        struct sockaddr_in _address;
        int _sock;
        int _connection;

    public:
        SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
        virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
        void test_connection(int);
        struct sockaddr_in get_address();
        int get_sock();
        int get_connection();
        void set_connection(int con);
};

#endif