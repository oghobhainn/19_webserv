#include "TestServer.hpp"

TestServer::TestServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    launch();
}

TestServer::TestServer(int port) : SimpleServer(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 10)
{
    launch();
}

void TestServer::accepter()
{
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    _new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen); 
    read(_new_socket, _buffer, 30000);
}

void TestServer::handler()
{
    std::cout << _buffer << std::endl;

	t_http_request	http_req_struct;
    std::string     http_response_firstline;
    http_request_parser(_buffer, http_req_struct);

    Http_response test;
    test.Http_response::build_http_response(http_req_struct);
}

void TestServer::responder()
{
    const char *hello = "Hello from server";
    write(_new_socket, hello, strlen(hello));
    close(_new_socket);
}

void TestServer::launch()
{
    fd_set ready_sockets;
    fd_set current_socket;
    FD_ZERO(&current_socket);
	FD_SET(get_socket()->get_sock(), &current_socket);
    
    while(true)
    {
        ready_sockets = current_socket;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			std::cout << "Failure whit select " << std::endl;
			exit(EXIT_FAILURE);
		}
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                if (i == get_socket()->get_sock())
                {
                    std::cout << "=========== Waiting ==========" << std::endl;
                    accepter();
                    handler();
                    responder();
                    std::cout << "=========== Done ==========" << std::endl;
                }
            }
        }
    }
}