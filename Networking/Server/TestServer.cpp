// probleme de read et transformer _new_socket en fd_set

#include "TestServer.hpp"

TestServer::TestServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    launch();
}

TestServer::TestServer(int port) : SimpleServer(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 10)
{
    launch();
}

void TestServer::set_socket(int new_socket)
{
    _new_socket = new_socket;
}

int TestServer::get_new_socket()
{
    return (_new_socket);
}

void TestServer::accepter()
{
    // int sock;
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    set_socket(accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen));
    // sock = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    // recv(sock, _buffer, 2048, 0);
}

void TestServer::handler()
{
    std::cout << _buffer << std::endl;

    t_http_request  http_req_struct;
    std::string     http_response_firstline;
    http_request_parser(_buffer, http_req_struct);

    Http_response test;
    test.Http_response::build_http_response(http_req_struct);
}

void TestServer::readsocket()
{
    long ret;
    int msgsize = 0;

    if ((ret = recv(get_new_socket(), _buffer, 2048, 0)) == -1)
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
        std::cout << _buffer << std::endl;
    }
}

void TestServer::responder()
{
    const char *hello = "Hello from server";
    write(get_new_socket(), hello, strlen(hello));
    close(get_new_socket());
}

void TestServer::launch()
{
    fd_set ready_sockets;
    fd_set current_socket;
    int ret;

    // fd_set writing_socket;
    // fd_set reading_socket;

    FD_ZERO(&current_socket);
    FD_SET(get_socket()->get_sock(), &current_socket);


    while(true)
    {
        ready_sockets = current_socket;
        if ((ret = select(FD_SETSIZE, &ready_sockets, nullptr, nullptr, nullptr)) < 0)
        {
            std::cout << "Failure with select " << std::endl;
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &current_socket))
            {
                std::cout << "=========== Waiting ==========" << std::endl;
                if (i == get_socket()->get_sock())
                {
                    // this is a new connection
                    accepter();
                    FD_SET(get_new_socket(), &current_socket);
                }
                else
                {
                    // do whatever we do with the connection
                    std::cout << " step 0" << std::endl;
                    readsocket();
                    std::cout << " step 1" << std::endl;
                    handler();
                    std::cout << " step 2" << std::endl;
                    responder();
                    std::cout << " step 3" << std::endl;
                    FD_CLR(i, &current_socket);
                }
            }
        }
    }
}