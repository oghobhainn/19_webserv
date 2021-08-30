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
    launch(&serv_list);
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

std::list<Server>::iterator TestServer::find_server(int socket, std::list<class Server> *serv_list)
{
    std::list<Server>::iterator it;

    int i = 0;
    for (it = serv_list->begin(); it != serv_list->end(); ++it)
    {
        if (FD_ISSET(socket, &it->socket_client))
        {
            // std::cout << "string found : " << it->getFullStr() << std::endl;
            return(it);
        }
        i++;
    }
    std::cout << "Server not found" << std::endl;
    return(it);
}

int TestServer::accepter(int socket, std::list<class Server> *serv_list)
{
    int sock_tmp;
    std::list<Server>::iterator it;

    // std::cout << socket << std::endl;
    for (it = serv_list->begin(); it != serv_list->end(); ++it)
    {
        if (socket == it->getSocket()->get_sock())
            break ;
    }
    struct sockaddr_in address = it->getSocket()->get_address();
    int addrlen = sizeof(address);
    sock_tmp = accept(socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    // fcntl(sock_tmp, F_SETFL, O_NONBLOCK);
    it->addSocketClient(sock_tmp);
    return (sock_tmp);
}

void TestServer::handler(int socket, Server serv)
{
    Request     req(_buffer);

    std::cout << "=============================================================================================" << std::endl;
    PY("request : ");
    // std::cout << req << std::endl;
    std::cout << _buffer << std::endl;

    // Server          serv;
    //RequestConfig	requestConf;
	Response		response;
    response.call(req, serv);
    response = response.Response::buildResponse(req, serv);

    std::cout << "=============================================================================================" << std::endl;
    P("response : ");
    std::cout << response.getResponse() << std::endl;
    char char_response[response.getResponse().length() + 1];
    strcpy(char_response, response.getResponse().c_str()); 
    write(socket, char_response, strlen(char_response));
    close(socket);
}

void TestServer::readsocket(int socket)
{
    int ret = 0;
    char buff[100001];
    memset(buff, 0, 100001);
    ret = recv(socket, buff, 100000, 0);
    if (ret == -1)
        std::cout << "Error with recv" << std::endl;
    else if (ret == 0)
    {
        std::cout << "No bytes are there to read" << std::endl;
        // should close connection
    }
    else
    {
        buff[ret] = 0;
        strcpy(_buffer, buff);
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

void TestServer::launch(std::list<class Server> *serv_list)
{
    fd_set writing_socket;
    fd_set reading_socket;
    fd_set server_socket;
    int ret;
    int sock_tmp = 0;

	struct timeval timeout = {2, 0};
    server_socket = get_connecting_socket();
    FD_ZERO(&writing_socket);
    while(true)
    {
        ret = 0;
        // server_socket = get_connecting_socket();
        // writing_socket = server_socket;
        // reading_socket = server_socket;
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
            }
        }
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            // if i is in set_fd of read
            if (FD_ISSET(i, &reading_socket) && FD_ISSET(i, &server_socket))
            {
                // this is a new connection
                sock_tmp = accepter(i, serv_list);
                FD_SET(sock_tmp, &reading_socket);
            }
            // if i is in socket of write
            if (FD_ISSET(i, &reading_socket) && !FD_ISSET(i, &server_socket))
            {
                std::list<Server>::iterator it;
				it = find_server(i, serv_list);
                readsocket(i);
                handler(i, *it);
                responder(i);
                remove_connecting_socket(i);
                memset(_buffer, 0, 1000001);
                FD_CLR(i, &reading_socket);
            }
        }
    }
}