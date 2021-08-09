#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include "parsing.hpp"

typedef struct	s_hresp_firstline 
{
	std::string	method;
	int			status_code;
	std::string	status_message;
}				t_hresp_firstline;

typedef struct	s_resp_header
{
	
}				t_hresp_header;

typedef struct	s_hresp_body
{
	std::string	line;
}				t_hresp_body;

typedef struct	s_http_response
{
	t_hresp_firstline	firstline;
	t_hresp_header		header;
	t_hresp_body		body;
}				t_http_response;

//std::string		build_http_response(t_http_request const http_req_struct);

class	Http_response{

	public:
		Http_response();
		Http_response(Http_response const & src); // constructeur par copie
		//Http_response(int const n); // constructeur parametrique
		~Http_response();

		std::string const & get_firstline( void ) const;
		void				set_firstline( int const status_code );

		Http_response		build_http_response( t_http_request const http_req_struct);
	private:
		std::string		_firstline;
		std::string		_fl_method;
		int				_fl_status_code;
		std::string		_fl_status_message;

		std::string		_header_user_agent;
		std::string		_header_host;
		std::string		_header_accept;
		std::string		_header_accept_language;
		std::string		_header_accept_encoding;
		std::string		_header_referer;
		std::string		_header_connection;
		std::string		_header_upgrade_insecure_requests;
		std::string		_header_if_modified_since;
		std::string		_header_if_none_match;
		std::string		_header_content_type;
		std::string		_header_content_length;
		std::string		_header_cache_control;

		std::string		_body_content;
};

#endif