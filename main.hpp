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
#include "utils/utils.hpp"

#endif
