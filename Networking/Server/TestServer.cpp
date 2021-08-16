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
    // return (_new_socket);
    // read(_new_socket, _buffer, 30000);
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

void TestServer::readsocket()
{
	long valread;
    int msgsize = 0;

    while ((valread = read(_new_socket, _buffer + msgsize, sizeof(_buffer) - msgsize - 1)))
	{
		msgsize += valread;
		if (msgsize > 1024 - 1 || _buffer[msgsize - 1] == '\n')
			break ;
	}
	_buffer[msgsize - 1] = 0;
	std::cout << _buffer << std::endl;
	if (valread < 0)
	{
		std::cout << "No bytes are there to read" << std::endl;
	}
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
			std::cout << "ret = " << ret << std::endl;
            std::cout << "Failure with select " << std::endl;
			exit(EXIT_FAILURE);
		}
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            std::cout << i << std::endl;
            if (FD_ISSET(i, &ready_sockets))
            {
                std::cout << "=========== Waiting ==========" << std::endl;
                if (i == get_socket()->get_sock())
                {
                    // this is a new connection
                    std::cout << "new socket before connection: " << _new_socket << std::endl;
                    std::cout << "=========== New connection ==========" << std::endl;
                    accepter();
                    // readsocket();
                    // handler();
                    // responder();
                    FD_SET(_new_socket, &current_socket);
                }
                else 
                {
                    // do whatever we do with the connection
                    std::cout << "socket after connection and before doing something: " << _new_socket << "\n";
                    std::cout << "=========== doing somethings ==========" << std::endl;
                    std::cout << " step 0" << std::endl;
                    readsocket();
                    std::cout << " step 1" << std::endl;
                    handler();
                    std::cout << " step 2" << std::endl;
                    responder();
                    std::cout << " step 3" << std::endl;
                    FD_CLR(i, &current_socket);
                }
                std::cout << "=========== Done ==========" << std::endl;
            }
        }
    }
}