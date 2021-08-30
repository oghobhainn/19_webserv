#ifndef TestServer_hpp
#define TestServer_hpp

#include <stdio.h>
#include <string.h>
#include <vector>
#include <fcntl.h>

// #include "SimpleServer.hpp"
#include "SimpleServer.hpp"
#include "../Sockets/ListeningSocket.hpp"
#include "../../ConfigParsing/Config.hpp"
#include "../../Request/Request.hpp"
#include "../../Response/Response.hpp"
#include "../../Response/ResponseHeader.hpp"

class TestServer
{
    private:
        int _new_socket;
        fd_set _set_of_socket;
        char _buffer[1000001];
        ListeningSocket *_socket;


    public:
        TestServer();
        TestServer(int port);
        ~TestServer();

        void launch();
        void accepter();
        void readsocket();
        // TestServer();
        TestServer(std::list<class Server> serv_list);
        
        void launch(std::list<class Server> *serv_list);
        int accepter(int socket, std::list<class Server> *serv_list);
        void readsocket(int socket);
        void handler(int socket);
        void responder(int socket);

        int get_new_socket();
        void set_socket(int new_socket);
        fd_set get_connecting_socket();
        void add_connecting_socket(int connecting_socket);
        void remove_connecting_socket(int connecting_socket);

        std::list<Server>::iterator find_server(int socket_client, std::list<class Server> *serv_list);
        ListeningSocket *get_socket();
        ListeningSocket *create_sub_server(int domain, int service, int protocol, int port, u_long interface, int bklg);
};


#endif
