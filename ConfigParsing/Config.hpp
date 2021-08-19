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

class Location
{
	public:
		Location();
		~Location();

		bool					active;
		std::string				full_str;
		std::list<std::string>	file_extensions;
		size_t					max_body;
		std::string				root;
		std::string				index;
		bool                    get_method;
		bool					post_method;
		bool					delete_method;
        std::string             redirection;

        std::string             directory_listing;
        std::string             default_file_if_request_directory;
        // t_CGI CGI; //If equal to NULL no CGI server, but http static content server
};

class Server : public Location
{
    private:
        std::string             full_str;
        std::string             str_without_loc;
        std::string			    host;
        std::string			    port;
		std::string				root;
        std::string			    server_name;
        
        size_t				    body_size_limit;
        std::list<std::string>  locations;
        ListeningSocket         *_socket; ////////////////////////////
        fd_set                  socket_client; //////////////////////

        std::string             default_error_page;
        int				        client_body_size;
		int 					nb_loc;


    public:
        Server();
        ~Server();
		Location *locations;
        Server& operator=(Server const& copy);
        
        void setFullStr(std::string const str);
        std::string getFullStr() const;

        void setStrWithoutLoc(std::string const str);
        std::string getStrWithoutLoc() const;

		void setServerName(int const i);
		std::string getServerName() const;

        void setLocations(std::list<Location> const lst);
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

		void setRoot(std::string const str);
        std::string getRoot() const;

		void setNbLoc(int const i);
        int getNbLoc() const;

        void setDefaultErrorPage(std::string const str);
        std::string getDefaultErrorPage() const;

        void setClientBodySize(std::string const str);
        int getClientBodySize() const;

};


std::ostream& operator<<(std::ostream& os, const Server& item);


# endif