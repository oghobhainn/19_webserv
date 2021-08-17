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


#define P(x) std::cout << x << std::endl

typedef struct s_location
{
	std::string				full_str;
	bool					active;
	std::list<std::string>	file_extensions;
	std::string				directory;
	size_t					max_body;
	std::string				root;
	std::list<std::string>	index;
	// std::string				directory_listing;
	// std::string				default_file_if_request_directory;
	// std::string				file_upload_location;
	// std::string				FOUND_URL;
	// t_CGI					CGI;
}				t_location;

class Server : t_location
{
    private:
        std::string             full_str;
        std::string             str_without_loc;
        std::string			    host;
        std::string			    port;
		std::string				root;
        std::string			    server_name;
        size_t				    body_size_limit;
        std::list<t_location>	locations;

    public:
        Server();
        ~Server();
        Server& operator=(Server const& copy);
        
        void setFullStr(std::string const str);
        std::string getFullStr() const;

        void setStrWithoutLoc(std::string const str);
        std::string getStrWithoutLoc() const;

        void setLocations(std::list<t_location> const lst);
        void getLocations() const;

        void setHost(std::string const str);
        std::string getHost() const;

        void setPort(std::string const str);
        std::string getPort() const;

		void setRoot(std::string const str);
        std::string getRoot() const;
};


std::ostream& operator<<(std::ostream& os, const Server& item);


# endif