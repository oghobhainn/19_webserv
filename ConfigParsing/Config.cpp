#include "Config.hpp"

CGI::CGI()
{
	this->active = true;
	this->AUTH_TYPE = std::string("None");
	this->CONTENT_LENGTH = std::string("0");
	this->CONTENT_TYPE = std::string("None");
	this->GATEWAY_INTERFACE = std::string("None");
	this->PATH_INFO = std::string("None");
	this->PATH_TRANSLATED = std::string("None");
	this->QUERY_STRING = std::string("");
	this->REMOTE_ADDR = std::string("None");
	this->REMOTE_INDENT = std::string("None");
	this->REMOTE_USER = std::string("None");
	this->REQUEST_METHOD = std::string("None");
	this->REQUEST_URI = std::string("None");
	this->SCRIPT_NAME = std::string("0");
	// this->SERVER_NAME = conf.server_name;
	// this->SERVER_PORT = conf.port.front();
	this->SERVER_PROTOCOL = std::string("HTTP/1.1");
	this->SERVER_SOFTWARE = std::string("None");
	this->SECRET = std::string("None");
}

CGI::~CGI()
{

}


///////////////////////////////////////////////////////////////////////////////

Location::Location()
{
	this->active = false;
	this->get_method = false;
	this->post_method = false;
	this->delete_method = false;
}

Location::~Location()
{

}

////////////////////////////////////////////////////////////////////////////////

Server::Server() 
{
	this->full_str = "";
	this->str_without_loc = "";
    this->host = "None";
    this->port = "";
	this->server_name = "None";
	this->client_body_size = 30000;
}

Server::~Server()
{

}

Server& Server::operator=(Server const& copy)
{
	if (this != &copy)
	{

	}
	return *this;
}

void Server::setFullStr(std::string const str)
{
	this->full_str = str;
}

std::string Server::getFullStr() const
{
	return this->full_str;
}

void Server::getLocations() const
{
	for(int i = 0; i < nb_loc; ++i)
	{
		std::cout << "_____________IN ONE LOC___________" << std::endl;
		std::cout << "Full_str_loc: " << locations[i].full_str << std::endl;
		std::cout << "Root:         " << locations[i].root << std::endl;
		std::cout << "Index:        " << locations[i].index << std::endl;
		std::cout << "Redir:        " << locations[i].redirection << std::endl;
		std::cout << "GET:         	" << locations[i].get_method << std::endl;
		std::cout << "POST:         " << locations[i].post_method << std::endl;
		std::cout << "DELETE:       " << locations[i].delete_method << std::endl;
		std::cout << "Dir list:     " << locations[i].directory_listing << std::endl;
		std::cout << "Default file: " << locations[i].default_file_if_request_directory << std::endl;
		std::cout << "FILES front:  " << locations[i].file_extensions.front() << std::endl;
		std::cout << "FILES back:   " << locations[i].file_extensions.back() << std::endl;
		std::cout << "---CGI------" << std::endl;
		std::cout << "CGI PATH_INFO:" << locations[i]._CGI.PATH_INFO << std::endl;
	}
}

void Server::setServerName(int const i)
{
	std::string serv_name = "Server";
	std::string nb = std::to_string(i);

	this->server_name = serv_name + nb;
}

std::string Server::getServerName() const
{
	return this->server_name;
}

void Server::setHost(std::string const str)
{
	this->host = str;
}

std::string Server::getHost() const
{
	return this->host;
}

void Server::setPort(std::string const str)
{
	this->port = str;
}

std::string Server::getPort() const
{
	return this->port;
}

void Server::setRoot(std::string const str)
{
	this->root = str;
}

std::string Server::getRoot() const
{
	return this->root;
}

void Server::setNbLoc(int const i)
{
	this->nb_loc = i;
}

int Server::getNbLoc() const
{
	return this->nb_loc;
}

void Server::setDefaultErrorPage(std::string const str)
{
	this->default_error_page = str;
}

std::string Server::getDefaultErrorPage() const
{
	return this->default_error_page;
}

void Server::setClientBodySize(std::string const str)
{
	std::stringstream ss(str);
	size_t result;
	ss >> result;
	this->client_body_size = result;
}

size_t Server::getClientBodySize() const
{
	return this->client_body_size;
}

void Server::setCgiParam(std::string const str)
{
	this->cgi_param = str;
}

std::string Server::getCgiParam() const
{
	return this->cgi_param;
}

void Server::setCgiPass(std::string const str)
{
	this->cgi_pass = str;
}

std::string Server::getCgiPass() const
{
	return this->cgi_pass;
}

void Server::setPath(std::string const str)
{
	this->path = str;
}

std::string Server::getPath() const
{
	return this->path;
}

void Server::setStrWithoutLoc(std::string const str)
{
	this->str_without_loc = str;
}

std::string Server::getStrWithoutLoc() const
{
	return this->str_without_loc;
}

void Server::setSocket(ListeningSocket *socket)
{
	_socket = socket;
}

ListeningSocket *Server::getSocket()
{
	return (_socket);
}

void Server::addSocketClient(int socket)
{
	fd_set tmp;

	tmp = getSocketClient();
	FD_SET(socket, &tmp);
}

void Server::removeSocketClient(int socket)
{
	fd_set tmp;

	tmp = getSocketClient();
	FD_CLR(socket, &tmp);
}

fd_set Server::getSocketClient()
{
	return (socket_client);
}

void	Server::setAllowedMethods()
{
	if (get_method == true)
	{
		_allowed_methods.insert("GET");
	}
	if (post_method == true)
	{
		_allowed_methods.insert("POST");
	}
	if (delete_method == true)
	{
		_allowed_methods.insert("DELETE");
	}
}

std::set<std::string>	Server::getAllowedMethods() const
{
	return _allowed_methods;
}

void	Server::setContentLocation(const std::string &path)
{
	this->content_location = path;
}

std::string	Server::getContentLocation() const
{
	return this->content_location;
}
//////////////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const Server& item)
{
	item.getLocations();
    return os << "NODE \n" << "full_str : " << item.getFullStr();
}