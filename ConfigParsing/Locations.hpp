#ifndef LOCATIONS_HPP
#define LOCATIONS_HPP


#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <list>
#include <sys/wait.h>
#include <dirent.h>

class internal_server_error_exc : public std::exception
{
};

typedef struct s_location
{
	bool active;
	std::list<std::string> file_extensions;
	std::string directory;
	std::list<std::string> http_methods;
	size_t max_body;
	std::string root;
	std::list<std::string> index;
	std::string  directory_listing;
	std::string default_file_if_request_directory;
	std::string file_upload_location;
	std::string FOUND_URL;
} t_location;


typedef struct s_header_fields
{
	std::list<std::string> Accept_Charsets;
	std::list<std::string> Accept_Language;
	std::list<std::string> Allow;
	std::list<std::string> Authorization;
	std::list<std::string> Content_Language;
	std::list<std::string> Content_Length;
	std::list<std::string> Content_Location;
	std::list<std::string> Content_Type;
	std::list<std::string> Date;
	std::list<std::string> Host;
	std::list<std::string> Last_Modified;
	std::list<std::string> Location;
	std::list<std::string> Referer;
	std::list<std::string> Retry_After;
	std::list<std::string> Server;
	std::list<std::string> Transfer_Encoding;
	std::list<std::string> User_Agent;
  std::list<std::string> WWW_Authenticate;
	std::list<std::string> X_Secret;
	std::string Header_Line;
	std::string Body;
} t_header_fields;

typedef struct s_http_req
{
	std::string complete_request;
	std::string method;
	std::string URL;
	t_location loc;
	std::string protocol_version;
	t_header_fields header_fields;
	std::string message_body;
	bool		error;
	bool		ready;
	bool		non_body_parsed;
	int			body_index;
	int			status_code;
} t_http_req;

typedef struct	s_server
{
	int server_socket;
	struct sockaddr_in address;
	int addrlen;
	int client_socket[200 + 1];
	int connected_socket;
	std::map<int, t_http_req> requests;
	std::map<int, std::string>	answer;
	unsigned int		fd_max;
}				t_server;

typedef struct s_config
{
	std::string host;
	std::list<std::string> port;
	std::string server_name;
	std::string root;
	std::string default_error_page;
	size_t body_size_limit;
	std::list<t_location> locations;
	std::list<std::string> index;
	t_server s;
} t_config;

void parse_config(std::string path, std::list<t_config> &ret);
void parse_http_request(t_http_req &ret, std::string &req, t_config &conf);
int find_first_two_line_returns(std::string const &req);
bool getlinecut(std::ifstream &fd, std::string &line);
bool check_line(std::string const &line, const std::string &comp);
std::string following_content(std::string &line, const std::string &after);
std::list<std::string> following_contents(std::string line, const std::string &after);
std::list<std::string> split(std::string text, std::string const &sp);
std::string parse_between(std::string &line, char cut, char cut2, bool between=true);
std::string parse_until(std::string &line, std::string const &until, bool all=false);
void show_conf(t_config &conf);
void show_locations(std::list<t_location> &locations);
void show_http_request(t_http_req &req);
void URL_to_local_path(t_http_req &req, t_config &conf);

#endif
