#ifndef TestServer_hpp
#define TestServer_hpp

#include <stdio.h>
#include <string.h>

#include "SimpleServer.hpp"
#include "../../main.hpp"

class TestServer : public SimpleServer
{
    private:
        int _new_socket;
        char _buffer[300000];
        // fd_set writing_socket;
        // fd_set reading_socket;
        

    public:
        TestServer();
        TestServer(int port);
        ~TestServer();

        void launch();
        void accepter();
        void readsocket();
        void handler();
        void responder();

        int get_new_socket();
        void set_socket(int new_socket);
};


#endif
