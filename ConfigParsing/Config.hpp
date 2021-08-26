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
#include <set>
#include <vector>
#include <sys/socket.h>
#include <errno.h>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <sstream>

#include "../Networking/Sockets/ListeningSocket.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */

#define PC(a, b) std::cout << a << b << std::endl;
#define PY(a) std::cout << YELLOW << a << RESET << std::endl;
#define PE(a) std::cout << RED << a << RESET << std::endl;
#define P(x) std::cout << x << std::endl

class CGI
{
    public:
        CGI();
        ~CGI();

        bool active;
        std::string PATH_INFO;
        std::string SCRIPT_NAME;
        std::string AUTH_TYPE;
        std::string CONTENT_LENGTH;
        std::string CONTENT_TYPE;
        std::string GATEWAY_INTERFACE;
        std::string PATH_TRANSLATED;
        std::string QUERY_STRING;
        std::string REMOTE_ADDR;
        std::string REMOTE_INDENT;
        std::string REMOTE_USER;
        std::string REQUEST_METHOD;
        std::string REQUEST_URI;
        // std::string SERVER_NAME;
        // std::string SERVER_PORT;
        std::string SERVER_PROTOCOL;
        std::string SERVER_SOFTWARE;
        std::string SECRET;
};

class Location : public CGI
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
        std::set<std::string>   _allowed_methods;

        std::string             redirection;
        std::string             directory_listing;
        std::string             default_file_if_request_directory;
        CGI                     _CGI;
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
        std::string             default_error_page;
        std::string             cgi_param;
        std::string             cgi_pass;
        std::string             path;
        int				        client_body_size;
		int 					nb_loc;
        fd_set                  socket_client;
        ListeningSocket         *_socket;
        std::string             content_location;

    public:
        Server();
        ~Server();
        Server operator=(Server const& copy);

		Location *locations;
        
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
        size_t getClientBodySize() const;

        void setAllowedMethods();
        std::set<std::string> getAllowedMethods() const;

        void setCgiParam(std::string const str);
        std::string getCgiParam() const;

        void setCgiPass(std::string const str);
        std::string getCgiPass() const;

        void setPath(std::string const str);
        std::string getPath() const;

        void setContentLocation(std::string const & path);
        std::string getContentLocation() const;
};


std::ostream& operator<<(std::ostream& os, const Server& item);

# endif