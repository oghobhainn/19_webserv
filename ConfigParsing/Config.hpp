#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <errno.h>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <sstream>

#include "../Networking/Sockets/ListeningSocket.hpp"


#define P(x) std::cout << x << std::endl

class Server
{
    private:
        std::string             full_str;
        std::string             str_without_loc;
        std::string			    host;
        std::string			    port;
        std::string			    server_name;
        size_t				    body_size_limit;
        std::list<std::string>  locations;
        ListeningSocket         *_socket; ////////////////////////////
        fd_set                  socket_client; //////////////////////

    public:
        Server();
        ~Server();
        Server& operator=(Server const& copy);
        
        void setFullStr(std::string const str);
        std::string getFullStr() const;

        void setStrWithoutLoc(std::string const str);
        std::string getStrWithoutLoc() const;

        void setLocations(std::list<std::string> const lst);
        void getLocations() const;

        void setHost(std::string const str);
        std::string getHost() const;

        void setPort(std::string const str);
        std::string getPort() const;

        void setSocket(ListeningSocket *socket); //////////////////////////
        ListeningSocket *getSocket(); ////////////////////////////////////

        void addSocketClient(int socket); //////////////////////////////
        void removeSocketClient(int socket); //////////////////////////
        fd_set getSocketClient(); ////////////////////////////////////
};

std::ostream& operator<<(std::ostream& os, const Server& item);


# endif