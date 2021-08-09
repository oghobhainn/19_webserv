#include "response.hpp"

Http_response::Http_response() { return ; }
Http_response::~Http_response() { return ; }
Http_response::Http_response( Http_response const & src )
{
	this->_firstline = src.get_firstline();
	return ;
}

void				Http_response::set_firstline(int const status_code)
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
		case 202:
			status_message = "202 Accepted";
			break;
		case 203:
			status_message = "203 Non-Authoritative Information";
			break;
		case 204:
			status_message = "204 No Content";
			break;
		case 205:
			status_message = "205 Reset Content";
			break;
		case 206:
			status_message = "206 Partial Content";
			break;
		//redirections
		case 300:
			status_message = "300 Multiple Choice";
			break;
		case 301:
			status_message = "301 Moved Permanently";
			break;
		case 302:
			status_message = "302 Found";
			break;
		case 303:
			status_message = "303 See Other";
			break;
		case 304:
			status_message = "304 Not Modified";
			break;
		//ERROR client-side
		case 400:
			status_message = "400 Bad Request";
			break;
		case 401:
			status_message = "401 Unauthorized";
			break;
		case 402:
			status_message = "402 Payment Required";
			break;
		case 403:
			status_message = "403 Forbidden";
			break;
		case 404:
			status_message = "404 Not Found";
			break;
		case 405:
			status_message = "405 Method not Allowed";
			break;
		case 406:
			status_message = "406 Not Acceptable";
			break;
		case 407:
			status_message = "407 Proxy Authentification Required";
			break;
		case 408:
			status_message = "408 Request Timeout";
			break;
		case 409:
			status_message = "409 Conflict";
			break;
		case 410:
			status_message = "410 Gone";
			break;
		case 411:
			status_message = "411 Length Required";
			break;
		case 412:
			status_message = "412 Precondition Failed";
			break;
		case 413:
			status_message = "413 Payload Too Large";
			break;
		case 414:
			status_message = "414 URL Too Long";
			break;
		case 415:
			status_message = "415 Unsupported Media Type";
			break;
		case 416:
			status_message = "416 Requested Range Not Satisfiable";
			break;
		case 417:
			status_message = "417 Expectation Failed";
			break;
		case 418:
			status_message = "418 I'm a teapot";
			break;
		//ERROR server-side
		case 500:
			status_message = "500 Internal Server Error";
			break;
		case 501:
			status_message = "501 Not Implemented";
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
			break;
		case 506:
			status_message = "506 Variant Also Negotiates";
			break;
		case 507:
			status_message = "507 Insufficient Storage";
			break;
	}
	this->_firstline = ("HTTP/1.1 " + status_message);
}

std::string const &	Http_response::get_firstline( void ) const { return this->_firstline; }

Http_response		Http_response::build_http_response(t_http_request const http_req_struct)
{
	Http_response	http_resp;
	http_resp.Http_response::set_firstline( 200 );
	//std::string response = "HTTP/1.1 200 OK\r\nContent-Type: Saluut,Bonsoir\r\nContent-Length: 42\r\n'This is a dope ass server'\r\n\r\n";

	return http_resp;
}