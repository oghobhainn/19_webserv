#ifndef PARSING_HPP
# define PARSING_HPP

#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <sys/select.h>
#include <string.h>
#include <fcntl.h>

#include "../ConfigParsing/Config.hpp"
#include "../Utils/utils.hpp"
#include "../ConfigParsing/Config.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../Response/ResponseHeader.hpp"

std::list<class Server> get_serv_list(std::string full_str);
void parse_loc(std::list<class Server> &serv_list);
std::list<class Server> parseConfig(std::string const path);

#endif