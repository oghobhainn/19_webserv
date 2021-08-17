#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <sstream>
#include <sys/time.h>
#include <sys/stat.h>
#include <fstream>

#include "ConfigParsing/Config.hpp"
#include "Networking/Server/SimpleServer.hpp"
#include "Networking/Server/TestServer.hpp"
#include "Networking/Sockets/SimpleSocket.hpp"
#include "Networking/Sockets/ListeningSocket.hpp"
#include "Networking/Sockets/ConnectingSocket.hpp"
#include "Networking/Sockets/BindingSocket.hpp"

#include "request_parser/request.hpp"
#include "response_builder/response.hpp"


/*
*** Utils
*/

std::vector<std::string> split(std::string s, const char delim);
int len(std::string str);
std::list<std::string> split_lst(std::string str, char seperator);
std::string trim(std::string str, std::string whitespace);
int countFreq(std::string pat, std::string txt);
int nthOccurrence(const std::string& str, const std::string& findMe, int nth);

#endif
