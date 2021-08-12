#ifndef PARSING_HPP
#define PARSING_HPP

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <sstream>

template< typename T, typename U>
void	P(T str, U s) { std::cout << str << s << std::endl; }

typedef struct	s_hreq_firstline
{
	std::string	method;
	std::string	urlpath;
	std::string	complete_urlpath;
	std::string	httpversion;
}				t_hreq_firstline;

typedef struct	s_hreq_header
{
	std::string user_agent;
	std::string host;
	std::string accept;
	std::string accept_language;
	std::string accept_encoding;
	std::string referer;
	std::string connection;
	std::string upgrade_insecure_requests;
	std::string if_modified_since;
	std::string if_none_match;
	std::string content_language;
	std::string content_type;
	std::string content_length;
	std::string content_location;
	std::string cache_control;
}				t_hreq_header;

typedef struct	s_hreq_body
{
	std::string	line;
}				t_hreq_body;

typedef struct	s_http_request
{
	t_hreq_firstline	firstline;
	t_hreq_header		header;
	t_hreq_body			body;
	int					status_code;
	bool				error;
}				t_http_request;


/*http_request_parser*/

void	http_request_parser(char *buffer, t_http_request &http_req_struct);

/* UTILS */
void	print_request_firstline(t_hreq_firstline const & hreq_firstline_struct);
void	print_request_header(t_hreq_header const & hreq_header_struct);

std::string	remove_whitespaces(std::string str);
void tokenize(std::string const &str, const char delim, std::vector<std::string> &out);
bool	is_line_in(std::string s1, std::string s2);


#endif