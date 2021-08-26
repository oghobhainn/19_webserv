#ifndef SimpleServer_hpp
#define SimpleServer_hpp

#include <stdio.h>
#include <unistd.h>

#include "../Sockets/ListeningSocket.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */

#define PC(a, b) std::cout << a << b << std::endl;
#define PY(a) std::cout << YELLOW << a << RESET << std::endl;
#define PE(a) std::cout << RED << a << RESET << std::endl;

class SimpleServer
{
    private:
        ListeningSocket *_socket;
        virtual void accepter() = 0;
        virtual void handler() = 0;
        virtual void responder() = 0;

    public:
        SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklg);
        virtual void launch() = 0;
        ListeningSocket *get_socket();
};


#endif