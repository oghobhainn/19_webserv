#include "TestServer.hpp"

// TestServer::TestServer()
// {
//     ListeningSocket *socket;

//     socket = create_sub_server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);
//     setSocket(socket);
//     add_connecting_socket(socket->get_listening());
//     launch();
// }

TestServer::TestServer(std::list<class Server> serv_list)
{
    ListeningSocket *socket;
    
    for (std::list<Server>::iterator it = serv_list.begin(); it != serv_list.end(); ++it)
    {
        socket = create_sub_server(AF_INET, SOCK_STREAM, 0, stoi(it->getPort()), INADDR_ANY, 10);
        it->setSocket(socket);
        add_connecting_socket(socket->get_sock());
        socket = nullptr;
    }
    launch(serv_list);
}

ListeningSocket *TestServer::create_sub_server(int domain, int service, int protocol, int port, u_long interface, int bklg)
{
     return(new ListeningSocket(domain, service, protocol, port, interface, bklg));
}

// void TestServer::set_socket(int new_socket)
// {
//     _new_socket = new_socket;
// }

void TestServer::add_connecting_socket(int connecting_socket)
{
    FD_SET(connecting_socket, &_set_of_socket);
}

// int TestServer::get_new_socket()
// {
//     return (_new_socket);
// }

fd_set TestServer::get_connecting_socket()
{
    return (_set_of_socket);
}

int TestServer::accepter(int socket, std::list<class Server> serv_list)
{
    int sock_tmp;
    std::list<Server>::iterator it;

    for (it = serv_list.begin(); it != serv_list.end(); ++it)
    {
        if (socket == it->getSocket()->get_sock())
            break ;
    }
    struct sockaddr_in address = it->getSocket()->get_address();
    int addrlen = sizeof(address);
    sock_tmp = accept(it->getSocket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    it->addSocketClient(sock_tmp);
    return (sock_tmp);
}

void TestServer::handler()
{
    t_http_request  http_req_struct;
    std::string     http_response_firstline;
    http_request_parser(_buffer, http_req_struct);

    Http_response test;
    test.Http_response::build_http_response(http_req_struct);

    print_response(test);

}

void TestServer::readsocket(int socket)
{
    long ret;
    int msgsize = 0;

    if ((ret = recv(socket, _buffer, 2048, 0)) == -1)
    {
        std::cout << "Error with recv" << std::endl;
    }
    else if (ret == 0)
    {
        std::cout << "No bytes are there to read" << std::endl;
        // should close connection
    }
    else
    {
        _buffer[msgsize - 1] = 0;
    }
}

void TestServer::responder(int socket)
{
    const char *hello = "Hello from server";
    write(socket, hello, strlen(hello));
    close(socket);
}

void TestServer::launch(std::list<class Server> serv_list)
{
    fd_set ready_sockets;
    fd_set reading_socket;
    fd_set writing_socket;
    int ret;
    int sock_tmp = 0;

    FD_ZERO(&writing_socket);
    reading_socket = get_connecting_socket();
    while(true)
    {
        std::cout << "=========== Waiting ==========" << std::endl;
        ready_sockets = reading_socket;
        if ((ret = select(FD_SETSIZE, &reading_socket, &writing_socket, nullptr, nullptr)) < 0)
        {
            std::cout << "Failure with select " << std::endl;
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &reading_socket) || FD_ISSET(i, &writing_socket))
            {
                // if i is in set_fd of read
                if (FD_ISSET(i, &reading_socket))
                {
                    // this is a new connection
                    sock_tmp = accepter(i, serv_list);
                    FD_SET(sock_tmp, &writing_socket);
                    FD_CLR(i, &reading_socket);
                }
                // if i is in socket of write
                else
                {
                    // do whatever we do with the connection
                    readsocket(i);
                    handler();
                    responder(i);
                    FD_CLR(i, &writing_socket);

                }
            }
        }
    }
}