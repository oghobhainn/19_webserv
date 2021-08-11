#include "response.hpp"

Http_response::Http_response() { return ; }
Http_response::~Http_response() { return ; }
Http_response::Http_response( Http_response const & src )
{
	this->_firstline = src.get_firstline();
	return ;
}

void	Http_response::set_firstline		( int const status_code )
{
	std::string	status_message;

	switch (status_code) {
		//Success
		case 200:
			status_message = "200 OK";
			break;
		case 201:
			status_message = "201 Created";
			break;
		//redirections
		case 301:
			status_message = "301 Moved Permanently";
			break;
		case 302:
			status_message = "302 Found";
			break;
		//ERROR client-side
		case 400:
			status_message = "400 Bad Request";
			break;
		case 401:
			status_message = "401 Unauthorized";
			break;
		case 403:
			status_message = "403 Forbidden";
			break;
		// case 404:
			// status_message = "404 Not Found";
			// break;
		case 405:
			status_message = "405 Method not Allowed";
			break;
		case 408:
			status_message = "408 Request Timeout";
			break;
		case 418:
			status_message = "418 I'm a teapot";
			break;
		//ERROR server-side
		case 500:
			status_message = "500 Internal Server Error";
			break;
		case 502:
			status_message = "502 Bad Gateway";
			break;
		case 503:
			status_message = "503 Service Unavailable";
			break;
		case 504:
			status_message = "504 Gateway Timeout";
			break;
		case 505:
			status_message = "505 HTTP Version Not Supported";
		default:
			status_message = "404 Not Found";
	}
	this->_firstline = ("HTTP/1.1 " + status_message);
}
void	Http_response::set_allow			( std::string allow ) { this->_allow = allow ;}
void	Http_response::set_server			( std::string server) { this->_server = server; }
void	Http_response::set_date				( void )
{
	char		buf[255];
	time_t		now = time(0);
	struct tm	timenow = *gmtime(&now);
	timenow.tm_hour += 2;
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z+2", &timenow);

	this->_date = std::string("Date: ") + std::string(buf);
}
// void	Http_response::set_content_type 	( std::string content_type ) { this->_content_type = content_type; }
// void	Http_response::set_content_language	( std::string content_language ) { this->_content_language = content_language; }
// void	Http_response::set_content_length	( std::string content_length ) { this->_content_length = content_length; }
// void	Http_response::set_content_location	( std::string content_location ) { this->_content_location = content_location; }
void	Http_response::set_location			( std::string location ) { this->_location = location; }
void	Http_response::set_last_modified	( std::string last_modified ) { this->_last_modified = last_modified; }
void	Http_response::set_retry_after		( std::string retry_after ) { this->_retry_after = retry_after; }
void	Http_response::set_www_authenticate	( std::string www_authenticate ) { this->_www_authenticate = www_authenticate; }


std::string const &	Http_response::get_firstline		( void ) const { return this->_firstline; }
std::string	const & Http_response::get_allow			( void ) const { return this->_allow; }
std::string const & Http_response::get_server			( void ) const { return this->_server; }
std::string const & Http_response::get_http_response	( void ) const { return this->_http_response; }
std::string const & Http_response::get_date				( void ) const { return this->_date; }
std::string const & Http_response::get_content_type 	( void ) const { return this->_content_type; }
std::string const & Http_response::get_content_language ( void ) const { return this->_content_language; }
std::string const & Http_response::get_content_length 	( void ) const { return this->_content_length; }
std::string const & Http_response::get_content_location ( void ) const { return this->_content_location; }
std::string const & Http_response::get_location			( void ) const { return this->_location; }
std::string const & Http_response::get_last_modified	( void ) const { return this->_last_modified; }
std::string const & Http_response::get_retry_after		( void ) const { return this->_retry_after; }
std::string const & Http_response::get_www_authenticate	( void ) const { return this->_www_authenticate; }


void	Http_response::init_response( int status_code, t_http_request const http_req )
{
	this->Http_response::set_firstline( status_code);
	if (status_code == 405) // method not allowed
		this->Http_response::set_allow( std::string("Allow: ") + std::string("GET, POST, DELETE") + "\r\n");
	this->Http_response::set_date();
	this->Http_response::set_server(std::string("Server: ") + std::string("Webserv/1.0"));		
	// this->Http_response::set_content_type( std::string("Content-type: ") + http_req.header.content_type );
	// this->Http_response::set_content_language( std::string("Content-language: ") + http_req.header.content_language );
	// this->Http_response::set_content_length( std::string("Content-length: ") + http_req.header.content_length );
	// this->Http_response::set_content_location( std::string("Content-location: ") + http_req.header.content_location );
	if (status_code % 300 >= 100 || status_code == 201)
		this->Http_response::set_location( "Location: " + std::string("TODO") + "\r\n");
	this->Http_response::set_last_modified(std::string("Last-Modified: ") + std::string("TODO"));		
	this->Http_response::set_retry_after(std::string("Retry-After: ") + std::string("TODO"));		
	this->Http_response::set_www_authenticate(std::string("WWW-Authenticate: ") + std::string("TODO"));		

}

std::string			Http_response::GET_method(t_http_request const http_req_struct)
{
	std::string		GET_response;

	GET_response = this->Http_response::get_firstline() + "\r\n"
					+ std::string(this->Http_response::get_allow())
					+ this->Http_response::get_date() + "\r\n"
					// + this->Http_response::get_content_type() + "\r\n"
					// + this->Http_response::get_content_language() + "\r\n"
					// + this->Http_response::get_content_length() + "\r\n"
					// + this->Http_response::get_content_location() + "\r\n"
					+ this->Http_response::get_last_modified() + "\r\n"
					+ this->Http_response::get_location()
					+ this->Http_response::get_retry_after() + "\r\n"
					+ this->Http_response::get_server() + "\r\n"
					+ this->Http_response::get_www_authenticate() + "\r\n"
					+ "\r\n";

	return GET_response;
}

void			Http_response::build_http_response(t_http_request const http_req_struct)
{


	int status_code = 405;


	this->init_response( status_code, http_req_struct );
	if (http_req_struct.firstline.method == std::string("GET"))
	{
		this->_http_response = GET_method( http_req_struct );
	}
	else
	{
		this->_http_response = (std::string("THIS IS THE ELSE #theDoors"));
	}
}