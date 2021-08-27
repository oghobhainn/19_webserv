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
        FD_ZERO(&it->socket_client);
        add_connecting_socket(socket->get_sock());
        socket = nullptr;
    }
    launch(serv_list);
}

ListeningSocket *TestServer::create_sub_server(int domain, int service, int protocol, int port, u_long interface, int bklg)
{
     return(new ListeningSocket(domain, service, protocol, port, interface, bklg));
}

TestServer::~TestServer()
{

}

void TestServer::set_socket(int new_socket)
{
    _new_socket = new_socket;
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

std::list<Server>::iterator TestServer::find_server(int socket_client, std::list<class Server> serv_list)
{
    std::list<Server>::iterator it;

    int i = 0;
    for (it = serv_list.begin(); it != serv_list.end(); ++it)
    {
        std::cout << "str found: " << it->getFullStr() << std::endl;
        // if (FD_ISSET(socket_client, &it->socket_client))
        if (socket_client == it->tmp_client)
        {
            std::cout << "string found" << it->getFullStr();
            return(it);
        }
        i++;
    }
    std::cout << "Server not found" << std::endl;
    return(it);
}

int TestServer::accepter(int socket, std::list<class Server> serv_list)
{
    int sock_tmp;
    std::list<Server>::iterator it;

    std::cout << socket << std::endl;
    for (it = serv_list.begin(); it != serv_list.end(); ++it)
    {
        if (socket == it->getSocket()->get_sock())
            break ;
    }
    struct sockaddr_in address = it->getSocket()->get_address();
    int addrlen = sizeof(address);
    // std::cout << "hello 1" << std::endl;
    sock_tmp = accept(socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    printf("==================== sock = %p \n", &sock_tmp);
    FD_SET(sock_tmp, &it->socket_client);
    it->tmp_client = sock_tmp;
    fcntl(sock_tmp, F_SETFL, O_NONBLOCK);
    // std::cout << "hello 2" << std::endl;
    // it->addSocketClient(sock_tmp);
    return (sock_tmp);
}

void TestServer::handler(int socket)
{
    Request     req(_buffer);

    PY("request : ");
    std::cout << req << std::endl;

    Server          serv;
    //RequestConfig	requestConf;
	Response		response;
    response.call(req, serv);
    response = response.Response::buildResponse(req, serv);

    P("response : ");
    std::cout << response.getResponse() << std::endl;
    char char_response[response.getResponse().length() + 1];
    strcpy(char_response, response.getResponse().c_str()); 
    write(socket, char_response, strlen(char_response));
    close(socket);
}

void TestServer::readsocket(int socket)
{
    long ret = 0;
    char buff[100001];
	std::cout << "Socket read " << socket << std::endl;
    if ((ret = recv(socket, buff, 100000, 0)) == -1)
    {
        std::cout << "buff: " << buff << std::endl;
        std::cout << "Error with recv" << std::endl;
    }
    else if (ret == 0)
    {
        std::cout << "No bytes are there to read" << std::endl;
        // should close connection
    }
    else
    {
        buff[ret - 1] = 0;
        strcpy(buff, _buffer);
    }
}

void TestServer::responder(int socket)
{
    if (socket == -45) // shutting error
    {
        const char *hello = "<!DOCTYPE html><html><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>";
        write(socket, hello, strlen(hello));
        close(socket);
    }
}

void TestServer::launch(std::list<class Server> serv_list)
{
    fd_set writing_socket;
    fd_set reading_socket;
    fd_set server_socket;
    int ret;
    int sock_tmp = 0;
    int i = 0;

	struct timeval timeout = {2, 0};
    server_socket = get_connecting_socket();
    FD_ZERO(&writing_socket);
    while(true)
    {
        ret = 0;
        // server_socket = get_connecting_socket();
        writing_socket = server_socket;
        reading_socket = server_socket;
        // FD_ZERO(&writing_socket);
        while (ret == 0)
        {
            writing_socket = server_socket;
            reading_socket = server_socket;
            ret = select(FD_SETSIZE, &reading_socket, &writing_socket, nullptr, &timeout);
            if (ret < 0)
            {
                std::cout << "Failure with select " << std::endl;
                exit(EXIT_FAILURE);
            }
            if (ret == 0)
            {
                FD_ZERO(&writing_socket);
                FD_ZERO(&reading_socket);
                i++;
            }
            // std::cout << "shit " << i << std::endl;
        }
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            // std::cout << i << std::endl;
            // if i is in set_fd of read
            if (FD_ISSET(i, &reading_socket) && FD_ISSET(i, &server_socket))
            {
				std::cout << "================================================================= reading socket : " << i << std::endl;
                // this is a new connection
                sock_tmp = accepter(i, serv_list);
                // fcntl(sock_tmp, F_SETFL, O_NONBLOCK);
                FD_SET(sock_tmp, &reading_socket);
				std::cout << "================================================================= writing socket after being accepted : " << sock_tmp << std::endl;
            }
            // if i is in socket of write
            if (FD_ISSET(i, &reading_socket) && !FD_ISSET(i, &server_socket))
            {
                std::list<Server>::iterator it;
                // do whatever we do with the connection
                std::cout << "================================================================= writing socket : " << i << std::endl;
				it = find_server(i, serv_list);
                readsocket(i);
				std::cout << "hello 1" << std::endl;
                handler(i);
				std::cout << "hello 2" << std::endl;
                responder(i);
				std::cout << "hello 3" << std::endl;
                // remove_connecting_socket(i);
                FD_CLR(i, &reading_socket);
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
//     while(true)
//     {
//         writing_socket = server_socket;
//         reading_socket = exec_socket;
//         // std::cout << sizeof(reading_socket) << std::endl;
// 		// std::cout << sizeof(writing_socket) << std::endl;
//         FD_ZERO(&exec_socket);
//         ret = select(FD_SETSIZE, &reading_socket, &writing_socket, nullptr, &timeout);
//         std::cout << ret << std::endl;
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
//                 // do whatever we do with the connection
//                 std::cout << "================================================================= writing socket : " << i << std::endl;
// 				sock_tmp = accepter(i, serv_list);
//                 FD_SET(sock_tmp, &exec_socket);
//                 readsocket(sock_tmp);
// 				// std::cout << "hello 1" << std::endl;
//                 handler(sock_tmp);
// 				// std::cout << "hello 2" << std::endl;
//                 responder(sock_tmp);
// 				// std::cout << "hello 3" << std::endl;
//                 remove_connecting_socket(sock_tmp);
//                 FD_CLR(sock_tmp, &exec_socket);
//             }
//         }
//     }
// }

// void TestServer::launch(std::list<class Server> serv_list)
// {
//     fd_set server_socket;
//     fd_set writing_socket;
//     fd_set reading_socket;
//     fd_set client_socket;
//     int ret;
//     int sock_tmp = 0;

// 	struct timeval timeout = {1, 0};
//     server_socket = get_connecting_socket();
//     FD_ZERO(&client_socket);
//     while(true)
//     {
//         FD_ZERO(&writing_socket);
//         FD_ZERO(&reading_socket);
//         ret = select(FD_SETSIZE, &reading_socket, &writing_socket, nullptr, &timeout);
//         if (ret < 0)
//         {
//             std::cout << "Failure with select " << std::endl;
//             exit(EXIT_FAILURE);
//         }
// 		std::cout << "hello 1" << std::endl;
//         if (ret == 0)
//             launch(serv_list);
//         std::cout << "hello 2" << std::endl;
//         for (int i = 0; i < FD_SETSIZE; i++)
//         {
//             // std::cout << i << std::endl;
//             /////////////////// solution transformer fd_set en tab de set
//             if (FD_ISSET(i, &reading_socket) || FD_ISSET(i, &reading_socket))
//             {
//                 std::cout << i << std::endl;
//                 if (FD_ISSET(i, &reading_socket))
//                 // do whatever we do with the connection
//                 {
//                     std::cout << "================================================================= reading socket : " << i << std::endl;
// 				    sock_tmp = accepter(i, serv_list);
//                     FD_SET(sock_tmp, &client_socket);
//                 }
//                 else
//                 {
//                     std::cout << "================================================================= writing socket : " << i << std::endl;
//                     readsocket(i);
// 				    // std::cout << "hello 1" << std::endl;
//                     handler(i);
// 				    // std::cout << "hello 2" << std::endl;
//                     responder(i);
// 				    // std::cout << "hello 3" << std::endl;
//                     remove_connecting_socket(i);
//                     FD_CLR(i, &client_socket);
//                 }
//             }
//         }
//     }
// }
