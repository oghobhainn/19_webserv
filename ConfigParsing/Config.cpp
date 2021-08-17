#include "Config.hpp"

Location::Location()
{
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

// void Server::setLocations(std::list<Location> const lst)
// {
// 	this->locations = lst;
// }

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
		std::cout << "GET:         	" << locations[i].get_method << std::endl;
		std::cout << "POST:         " << locations[i].post_method << std::endl;
		std::cout << "DELETE:       " << locations[i].delete_method << std::endl;
		std::cout << "---" << std::endl;
	}
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
	// item.getLocations();
    return os << "NODE \n" << "full_str : " << item.getFullStr();
}