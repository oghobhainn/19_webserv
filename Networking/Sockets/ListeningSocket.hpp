#ifndef ListeningSocket_hpp
#define ListeningSocket_hpp

#include <stdio.h>
#include "BindingSocket.hpp"

class ListeningSocket : public BindingSocket
{
    private:
        int _backlog;
        int _listening;

    public:
        ListeningSocket(int domain, int service, int protocol, int port, long interface, int bklg);
        void start_listening();
        int get_blacklog();
        int get_listening();
};


#endif