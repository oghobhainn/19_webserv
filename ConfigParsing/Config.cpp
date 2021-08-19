#include "Config.hpp"

Server::Server()
{
	this->full_str = "";
	this->str_without_loc = "";
    this->host = "None";
    this->port = "";
	this->server_name = "None";
	this->body_size_limit = sizeof(size_t);
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

void Server::setLocations(std::list<std::string> const lst)
{
	this->locations = lst;
}

std::string Server::getFullStr() const
{
	return this->full_str;
}

void Server::getLocations() const
{
	// for (std::list<std::string>::iterator it = this->locations.begin(); it != this->locations.end(); ++it)
	// 	std::cout << *it << std::endl;
	std::string test1, test2;
	test1 = this->locations.front();
	test2 = this->locations.back();
	std::cout << "first loc: " << test1 << std::endl;
	std::cout << "last loc: " << test2 <<  std::endl;
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

//////////////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const Server& item)
{
	// item.getLocations();
    return os << "NODE \n" << "full_str : " << item.getFullStr();
}