#ifndef REQUEST_UTILS_HPP
#define REQUEST_UTILS_HPP

#include "../main.hpp"
#include <sstream>

int								countSubstr(const std::string& str, const std::string& sub);
int								checkStart(const std::string& str, const std::string& end);
int								checkEnd(const std::string& str, const std::string& end);
std::string						readKey(char *line);
std::string						readKey(const std::string& line);
std::string						readValue(char *line);
std::string						readValue(const std::string& line);
std::vector<std::string>		request_split(const std::string& str, char c);
std::string&					strip(std::string& str, char c);
std::string&					to_upper(std::string& str);
std::string&					to_lower(std::string& str);
std::string&					pop(std::string& str);
std::string&					capitalize(std::string& str);
std::string						to_string(size_t n);

#endif