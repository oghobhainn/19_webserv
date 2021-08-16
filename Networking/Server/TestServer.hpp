#ifndef TestServer_hpp
#define TestServer_hpp

#include <stdio.h>
#include <string.h>

#include "SimpleServer.hpp"
#include "../../main.hpp"

class TestServer : public SimpleServer
{
    private:
        char _buffer[30000];
        int _new_socket;
        // fd_set writing_socket;
        // fd_set reading_socket;
        
        void accepter();
        void readsocket();
        void handler();
        void responder();

    public:
        TestServer();
        TestServer(int port);
        void launch();

};


#endif
