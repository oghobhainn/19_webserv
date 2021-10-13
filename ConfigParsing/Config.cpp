#include "Config.hpp"

Location::Location()
{
	this->active = false;
	this->get_method = false;
	this->post_method = false;
	this->delete_method = false;
	this->autoindex = true;
	this->file_upload_location = "./default/";
}

Location::~Location()
{

}

Server::Server() 
{
	this->full_str = "";
	this->str_without_loc = "";
    this->host = "None";
    this->port = "";
	this->server_name = "None";
	this->client_body_size = 30000;
	this->host = "127.0.0.1";
	this->default_error_page = "./default/404.html";
	this->get_method = false;
    this->post_method = false;
    this->delete_method = false;
}

Server::~Server()
{

}

Server& Server::operator=(Server const& copy)
{
	if (this != &copy)
	{
        this->full_str = copy.full_str ;
        this->str_without_loc = copy.str_without_loc;
        this->port = copy.port;
        this->root = copy.root;
        this->server_name = copy.server_name;
        this->default_error_page = copy.default_error_page;
        this->path = copy.path;
        this->client_body_size = copy.client_body_size;
        this->nb_loc = copy.nb_loc;
        this->socket_client = copy.socket_client;
        // this->_socket = copy._socket;
        this->content_location = copy.content_location;
		this->index = copy.index;
		this->locations = copy.locations;
		this->get_method = copy.get_method;
        this->post_method = copy.post_method;
        this->delete_method = copy.delete_method;
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
		std::cout << "Autoindex:    " << locations[i].autoindex << std::endl;
		std::cout << "Dir list:     " << locations[i].file_upload_location << std::endl;
		std::cout << "Default file: " << locations[i].default_file_if_request_directory << std::endl;
		std::cout << "FILES front:  " << locations[i].file_extensions.front() << std::endl;
		std::cout << "FILES back:   " << locations[i].file_extensions.back() << std::endl;
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

void Server::addSocketClient(int socket)
{
	FD_SET(socket, &socket_client);
}

void Server::removeSocketClient(int socket)
{
	FD_CLR(socket, &socket_client);
}

fd_set Server::getSocketClient()
{
	return (socket_client);
}

void Server::setSocketClient(fd_set socket)
{
	FD_ZERO(&socket_client);
	socket_client = socket;
}

void	Server::setContentLocation(const std::string &path)
{
	this->content_location = path;
}

std::string	Server::getContentLocation() const
{
	return this->content_location;
}

void Server::setAutoIndex(std::string const str)
{
	if (str.find("off") != std::string::npos)
		this->autoindex = false;
	else
		this->autoindex = true;
}

bool Server::getAutoIndex()
{
	return this->autoindex;
}

void Server::setIndex(std::string const str)
{
	this->index = str;
}

std::string Server::getIndex() const
{
	return this->index;
}

void Server::setGetMethod(bool x)
{
    if (x == true)
        this->get_method = true;
}
bool Server::getGetMethod()
{
    return this->get_method;
}
void Server::setPostMethod(bool x)
{
    if (x == true)
        this->post_method = true;
}
bool Server::getPostMethod()
{
    return this->post_method;
}
void Server::setDeleteMethod(bool x)
{
    if (x == true)
        this->delete_method = true;
}
bool Server::getDeleteMethod()
{
    return this->delete_method;
}

std::ostream& operator<<(std::ostream& os, const Server& item)
{
    return os << "full_str : " << item.getFullStr();
}

