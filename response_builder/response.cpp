#include "response.hpp"

Http_response::Http_response() { return ; }
Http_response::~Http_response() { return ; }

void	Http_response::set_firstline		( int const & status_code )
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
void	Http_response::set_allow			( std::string const & allow ) { this->_allow = allow ;}
void	Http_response::set_server			( std::string const & server) { this->_server = server; }
void	Http_response::set_date				( void )
{
	char		buf[255];
	time_t		now = time(0);
	struct tm	timenow = *gmtime(&now);
	timenow.tm_hour += 2;
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z+2", &timenow);

	this->_date = std::string("Date: ") + std::string(buf);
}
void	Http_response::set_content_type 	( std::string const & url )
{
	// https://stackoverflow.com/questions/3631153/how-come-some-site-urls-do-not-include-a-file-extension
	//default is html, if there is no file extension then it should have been produced by php, python or whatever but the result is html
	// https://developer.mozilla.org/fr/docs/Web/HTTP/Basics_of_HTTP/MIME_types
	std::string				base = "Content-type: ";
	std::string				filename;
	std::string				content_type;
	std::string::size_type	idx;
	std::string 			extension;

	idx = url.rfind('/');
	if (idx != std::string::npos)
		filename = url.substr(idx + 1);
	
	idx = filename.rfind('.');
	if (idx != std::string::npos)
		extension = filename.substr(idx + 1);
	else
		extension = "html";
	
	if (extension == "html")
		this->_content_type = base + std::string("text/html");
	else if (extension == "css")
		this->_content_type = base + std::string("text/css");
	if (extension == "png")
		this->_content_type = base + std::string("image/png");
	else if (extension == "jpeg")
		this->_content_type = base + std::string("image/jpeg");
//audio...
//video...
//app...
	else
		this->_content_type = base + std::string("text/plain");
}
// void	Http_response::set_content_language	( std::string const & content_language ) { this->_content_language = content_language; }
// void	Http_response::set_content_length	( std::string const & content_length ) { this->_content_length = content_length; }
// void	Http_response::set_content_location	( std::string const & content_location ) { this->_content_location = content_location; }
void	Http_response::set_location			( std::string const & location ) { this->_location = location; }
void	Http_response::set_last_modified	( std::string const & filename )
{
	char 		buf[255];
	struct tm	timenow;
	struct stat	file;

	stat(filename.c_str(), &file);
	strptime(ctime(&file.st_mtime), "%a %b %d %T %Y", &timenow);
	strftime(buf, 255, "%a, %d %b %H:%M:%S UTC+2", &timenow);

	this->_last_modified = std::string("Last-modified: ") + std::string(buf);	
}
// void	Http_response::set_retry_after		( std::string const & retry_after ) { this->_retry_after = retry_after; }
// void	Http_response::set_www_authenticate	( std::string const & www_authenticate ) { this->_www_authenticate = www_authenticate; }

std::string const &	Http_response::get_firstline		( void ) const { return this->_firstline; }
std::string	const & Http_response::get_allow			( void ) const { return this->_allow; }
std::string const & Http_response::get_server			( void ) const { return this->_server; }
std::string const & Http_response::get_http_response	( void ) const { return this->_http_response; }
std::string const & Http_response::get_date				( void ) const { return this->_date; }
std::string const & Http_response::get_content_type 	( void ) const { return this->_content_type; }
// std::string const & Http_response::get_content_language ( void ) const { return this->_content_language; }
// std::string const & Http_response::get_content_length 	( void ) const { return this->_content_length; }
// std::string const & Http_response::get_content_location ( void ) const { return this->_content_location; }
std::string const & Http_response::get_location			( void ) const { return this->_location; }
std::string const & Http_response::get_last_modified	( void ) const { return this->_last_modified; }
// std::string const & Http_response::get_retry_after		( void ) const { return this->_retry_after; }
// std::string const & Http_response::get_www_authenticate	( void ) const { return this->_www_authenticate; }
std::string const & Http_response::get_body				( void ) const { return this->_body; }


void	Http_response::init_response( int status_code, t_http_request const http_req )
{
	this->Http_response::set_firstline( status_code);
	if (status_code == 405) // method not allowed
		this->Http_response::set_allow( std::string("Allow: GET, POST, DELETE") + "\r\n");
	this->Http_response::set_date();
	this->Http_response::set_server(std::string("Server: Webserv/1.0"));		
	this->Http_response::set_content_type( http_req.firstline.urlpath );
	// this->Http_response::set_content_language( std::string("Content-language: ") + http_req.header.content_language );
	// this->Http_response::set_content_length( std::string("Content-length: ") + http_req.header.content_length );
	// this->Http_response::set_content_location( std::string("Content-location: ") + http_req.header.content_location );
	if (status_code % 300 >= 100 || status_code == 201)
		this->Http_response::set_location( "Location: " + std::string("TODO") + "\r\n");
	this->Http_response::set_last_modified(std::string("Last-Modified: ") + std::string("TODO"));		
	// this->Http_response::set_retry_after(std::string("Retry-After: ") + std::string("TODO"));		
	// this->Http_response::set_www_authenticate(std::string("WWW-Authenticate: ") + std::string("TODO"));		

}

std::string			Http_response::GET_method(t_http_request const http_req_struct)
{
	std::ifstream	fd;
	std::string		GET_response;

	fd.open(http_req_struct.firstline.complete_urlpath);
	if (!fd.is_open())
	{
		fd.close();
		std::cout << "Error: file opening" << std::endl;
		return " BIG BAD ERROR OCCURED WHILE OPENING THE FILE -> 404 not found";
		//TODO - construire message d'erreur et le renvoyer
		//return error_page(404, http_req_struct.firstline.method, conf);
	}
	std::cout << "didnt exit, sooooo i guess we found the file?" << std::endl;
	GET_response = this->Http_response::get_firstline() + "\r\n"
					+ std::string(this->Http_response::get_allow())
					+ this->Http_response::get_date() + "\r\n"
					+ this->Http_response::get_content_type() + "\r\n"
					// + this->Http_response::get_content_language() + "\r\n"
					// + this->Http_response::get_content_length() + "\r\n"
					// + this->Http_response::get_content_location() + "\r\n"
					+ this->Http_response::get_last_modified() + "\r\n"
					+ this->Http_response::get_location()
					// + this->Http_response::get_retry_after() + "\r\n"
					+ this->Http_response::get_server() + "\r\n"
					// + this->Http_response::get_www_authenticate() + "\r\n"
					+ "\r\n"
					+ this->get_body() + "\r\n";
	fd.close();

	return GET_response;
}

std::string		Http_response::POST_method(t_http_request const http_req_struct)
{
	std::string		POST_response;

	if (http_req_struct.status_code == 200)
	{
		POST_response = this->get_firstline() + "\r\n"
					+ "\r\n"
					+ this->get_body() + "\r\n";
	}

	return POST_response;
}

std::string		Http_response::DELETE_method(t_http_request const http_req_struct)
{
	std::string		DELETE_response;

	if (http_req_struct.status_code == 200)
	{
		DELETE_response = this->get_firstline() + "\r\n"
						+ "\r\n"
						+ "THIS IS MY BODYYY\r\n";
	}
	return DELETE_response;
}

void			Http_response::build_http_response(t_http_request const http_req_struct)
{
	if (http_req_struct.error == false)
	{
		this->init_response( http_req_struct.status_code, http_req_struct );
		if (http_req_struct.firstline.method == std::string("GET"))
		{
			this->_http_response = GET_method( http_req_struct );
		}
		else if (http_req_struct.firstline.method == std::string("POST"))
		{
			this->_http_response = POST_method( http_req_struct );
		}
		else if (http_req_struct.firstline.method == std::string("DELETE"))
		{
			this->_http_response = DELETE_method( http_req_struct );
		}
		else
		{
			this->_http_response = (std::string("THIS IS THE ELSE #theDoors"));
		}
	}
	//else
	//	build_error_page(http_req_struct.status_code, http_req_struct.firstline.method);
	
//	P("\t\tResponse: \n", this->_http_response);
}
