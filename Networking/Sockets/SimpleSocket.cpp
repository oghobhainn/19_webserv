#include "SimpleSocket.hpp"

SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
    // define address structure
    _address.sin_family = domain;
        // port int's bytes are in an order for the host system. htons convert it for the network
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    // Establish socket
    _sock = socket(domain, service, protocol);
    test_connection(_sock);
}

void SimpleSocket::test_connection(int item_to_test)
{
    if (item_to_test < 0)
    {
        perror("Failed to connect");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in SimpleSocket::get_address()
{
    return _address;
}

int SimpleSocket::get_connection()
{
    return _connection;
}

int SimpleSocket::get_sock()
{
    return _sock;
}

void SimpleSocket::set_connection(int con)
{
    _connection = con;
}