#include "Config.hpp"

Location::Location()
{
	this->get_method = false;
	this->post_method = false;
	this->delete_method = false;
	this->active = false;
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
		std::cout << "---IN ONE LOC---" << std::endl;
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
		std::cout << "---" << std::endl;
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
	int nb = std::stoi(str);
	this->client_body_size = nb;
}

int Server::getClientBodySize() const
{
	return this->client_body_size;
}

void Server::setStrWithoutLoc(std::string const str)
{
	this->str_without_loc = str;
}

std::string Server::getStrWithoutLoc() const
{
	return this->str_without_loc;
}


//////////////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const Server& item)
{
	item.getLocations();
    return os << "NODE \n" << "full_str : " << item.getFullStr();
}