#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <iostream>
#include <sys/time.h>

#include "parsing.hpp"

//https://fr.wikipedia.org/wiki/Liste_des_codes_HTTP
//https://stackoverflow.com/questions/7548759/generate-a-date-string-in-http-response-date-format-in-c

class	Http_response{

	public:
		Http_response();
		Http_response(Http_response const & src);
		//Http_response(int const n); // constructeur parametrique
		~Http_response();

		std::string const & get_firstline		( void ) const;
		std::string	const & get_allow			( void ) const;
		std::string const & get_server			( void ) const;
		std::string const & get_http_response	( void ) const;
		std::string const & get_date			( void ) const;
		std::string const & get_content_type	( void ) const;
		std::string const & get_content_language( void ) const;
		std::string const & get_content_length 	( void ) const;
		std::string const & get_content_location( void ) const;
		std::string const & get_location		( void ) const;
		std::string const & get_last_modified	( void ) const;
		std::string const & get_retry_after		( void ) const;
		std::string const & get_www_authenticate( void ) const;

		void	set_firstline			( int const status_code );
		void	set_allow				( std::string allow );
		void	set_server				( std::string server );
		void	set_date				( void );
		// void	set_content_language	( std::string content_language );
		// void	set_content_type		( std::string content_type );
		// void	set_content_length		( std::string content_length );
		// void	set_content_location	( std::string content_location );
		void	set_location			( std::string location );
		void	set_last_modified		( std::string last_modified );
		void	set_retry_after			( std::string retry_after );
		void	set_www_authenticate	( std::string www_authenticate );

		void				init_response( int status_code, t_http_request const http_req );
		void				build_http_response( t_http_request const http_req_struct);
		std::string			GET_method( t_http_request const http_req_struct );
	
	private:
		std::string		_firstline;
		std::string		_fl_method;
		int				_fl_status_code;
		std::string		_fl_status_message;

		std::string		_allow;
		std::string		_date;
		std::string		_content_language;
		std::string		_content_length;
		std::string		_content_location;
		std::string		_content_type;
		std::string		_last_modified;
		std::string		_location;
		std::string		_retry_after;
		std::string		_server;
		std::string		_www_authenticate;

		std::string		_http_response;

		std::string		_body;
};

#endif