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


void TestServer::add_connecting_socket(int connecting_socket)
{
    FD_SET(connecting_socket, &_set_of_socket);
}

void TestServer::remove_connecting_socket(int connecting_socket)
{
    FD_CLR(connecting_socket, &_set_of_socket);
}

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
    sock_tmp = accept(socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
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
    fd_set writing_socket;
    fd_set reading_socket;
    fd_set server_socket;
    fd_set exec_socket;
    int ret;
    int sock_tmp = 0;

	struct timeval timeout = {1, 0};
    server_socket = get_connecting_socket();
    while(true)
    {
        writing_socket = server_socket;
        reading_socket = server_socket;
        FD_ZERO(&exec_socket);
        ret = select(FD_SETSIZE, &reading_socket, &writing_socket, nullptr, &timeout);
        if (ret < 0)
        {
            std::cout << "Failure with select " << std::endl;
            exit(EXIT_FAILURE);
        }
        if (ret == 0)
            launch(serv_list);

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            // std::cout << i << std::endl;
            if (FD_ISSET(i, &server_socket) || FD_ISSET(i, &exec_socket))
            {
                // do whatever we do with the connection
                std::cout << "================================================================= writing socket : " << i << std::endl;
				sock_tmp = accepter(i, serv_list);
                FD_SET(sock_tmp, &exec_socket);
                readsocket(sock_tmp);
				std::cout << "hello 1" << std::endl;
                handler();
				std::cout << "hello 2" << std::endl;
                responder(sock_tmp);
				std::cout << "hello 3" << std::endl;
                remove_connecting_socket(sock_tmp);
                FD_CLR(sock_tmp, &exec_socket);
            }
        }
    }
}

// void TestServer::launch(std::list<class Server> serv_list)
// {
//     fd_set writing_socket;
//     fd_set reading_socket;
//     fd_set server_socket;
//     fd_set exec_socket;
//     int ret;
//     int sock_tmp = 0;

// 	struct timeval timeout = {1, 0};
//     server_socket = get_connecting_socket();
//     writing_socket = server_socket;
//     reading_socket = server_socket;
//     while(true)
//     {
//         FD_ZERO(&exec_socket);
//         ret = select(FD_SETSIZE, &reading_socket, &writing_socket, nullptr, &timeout);
//         if (ret < 0)
//         {
//             std::cout << "Failure with select " << std::endl;
//             exit(EXIT_FAILURE);
//         }
//         if (ret == 0)
//             launch(serv_list);
//         for (int i = 0; i < FD_SETSIZE; i++)
//         {
//             // std::cout << i << std::endl;
//             if (FD_ISSET(i, &server_socket) || FD_ISSET(i, &exec_socket))
//             {
//                 // if i is in set_fd of read
//                 if (FD_ISSET(i, &server_socket))
//                 {
// 					std::cout << "================================================================= reading socket : " << i << std::endl;
//                     // this is a new connection
//                     sock_tmp = accepter(i, serv_list);
//                     FD_SET(sock_tmp, &exec_socket);
//                     // FD_CLR(i, &reading_socket);
// 					std::cout << "================================================================= writing socket after being accepted : " << sock_tmp << std::endl;
//                 }
//                 // if i is in socket of write
//                 else
//                 {
//                     // do whatever we do with the connection
//                     std::cout << "================================================================= writing socket : " << i << std::endl;
// 					readsocket(i);
// 					std::cout << "hello 1" << std::endl;
//                     handler();
// 					std::cout << "hello 2" << std::endl;
//                     responder(i);
// 					std::cout << "hello 3" << std::endl;
//                     remove_connecting_socket(i);
//                     FD_CLR(i, &exec_socket);
//                 }
//             }
//         }
//     }
// }