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
        
        void accepter();
        void handler();
        void responder();

    public:
        TestServer();
        TestServer(int port);
        void launch();

};


#endif
