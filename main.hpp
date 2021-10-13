#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <sys/time.h>
#include <sys/stat.h>
#include <fstream>
#include <sys/select.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "ConfigParsing/Config.hpp"
#include "Utils/utils.hpp"
#include "ConfigParsing/Config.hpp"
#include "Request/Request.hpp"
#include "Response/Response.hpp"
#include "Response/ResponseHeader.hpp"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define YELLOW  "\033[33m"      /* Yellow */

# define RECV_SIZE 65536
# define CGI_BUFSIZE 65536

#define PC(a, b) std::cout << a << b << std::endl;
#define PE(a) std::cout << RED << a << RESET << std::endl;

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
