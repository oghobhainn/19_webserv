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

void Server::setLocations(std::list<t_location> const lst)
{
	this->locations = lst;
}

std::string Server::getFullStr() const
{
	return this->full_str;
}

void Server::getLocations() const
{
	// std::list<t_location>::iterator it = this->locations.begin();
	// for (it = this->locations.begin(); it! = this->locations.end(); ++it)
    // 	std::cout << it->first << " => " << it->second << '\n';

	std::string test1, test2, test3, test4, test5, test6;
	test1 = this->locations.front().full_str;
	test2 = this->locations.back().full_str;
	test3 = this->locations.front().file_extensions.front();
	test4 = this->locations.front().file_extensions.back();
	test5 = this->locations.back().file_extensions.front();
	test6 = this->locations.back().file_extensions.back();
	std::cout << "--------------- IN LOCATIONS -----------------" << std::endl;
	std::cout << "first loc: " << test1 << std::endl;
	std::cout << "last loc: " << test2 <<  std::endl;
	std::cout << "-----------" << std::endl;
	std::cout << "first loc - first file ext: " << test3 <<  std::endl;
	std::cout << "first loc - last file ext: " << test4 <<  std::endl;
	std::cout << "-----------" << std::endl;
	std::cout << "last loc - first file ext: " << test5 <<  std::endl;
	std::cout << "last loc - last file ext: " << test6 <<  std::endl;
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