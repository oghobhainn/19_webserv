#include "Response.hpp"
#include "ResponseHeader.hpp"
#include "../Cgi/CgiHandler.hpp"

// Static Assets

// std::map<std::string, void (Response::*)(Request &, serverig &)>	Response::initMethods()
std::map<std::string, void (Response::*)(Request &, Server &)>	Response::initMethods()
{
	std::map<std::string, void (Response::*)(Request &, Server &)> map;

	map["GET"] = &Response::getMethod;
	map["POST"] = &Response::postMethod;
	map["DELETE"] = &Response::deleteMethod;

	return map;
}

std::map<std::string, void (Response::*)(Request &, Server &)> Response::_method = Response::initMethods();

// Member functions

void			Response::call(Request & request, Server & server)
{
	_error = server.getDefaultErrorPage();
	// _errorMap = server.getErrorPage();
	
	// _isAutoIndex = server.getAutoIndex(); //TODO
	// _isAutoIndex = server.getAutoIndex();
	
	_code = request.getRet();
	_host = server.getHost();
	_port = server.getPort();
	// _hostPort.host = server.getHostPort().host;
	// _hostPort.port = server.getHostPort().port;
	// _path = server.getPath();
	//_path = server.getPath(); // TODO

	if (server.getAllowedMethods().find(request.getMethod()) == server.getAllowedMethods().end())
		_code = 405;
	else if (server.getClientBodySize() < request.getBody().size())
		_code = 413;

	if (_code == 405 || _code == 413)
	{
		ResponseHeader	head;

		// _response = head.notAllowed(server.getAllowedMethods(), server.getLocations(), _code, server.getLang() + "\r\n";
		_response = head.notAllowed(server.getAllowedMethods(), server.getLocations(), _code, "\r\n");

		return ;
	}


	(this->*Response::_method[request.getMethod()])(request, server);
}

// Methods
void			Response::getMethod(Request & request, Server & server)
{
	ResponseHeader	head;

	if (server.getCgiPass() != "")
	{
		CgiHandler	cgi(request, server);
		size_t		i = 0;
		size_t		j = _response.size() - 2;

		_response = cgi.executeCgi(server.getCgiPass());

		while (_response.find("\r\n\r\n", i) != std::string::npos || _response.find("\r\n", i) == i)
		{
			std::string	str = _response.substr(i, _response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				_code = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-type: ") == 0)
				_type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (_response.find("\r\n", j) == j)
			j -= 2;

		_response = _response.substr(i, j - i);
	}
	else if  (_code == 200)
		_code = readContent();
	else
		_response = this->readHtml(_errorMap[_code]);
	if (_code == 500)
		_response = this->readHtml(_errorMap[_code]);

	// _response = head.getHeader(_response.size(), _path, _code, _type, server.getContentLocation(), server.getLang()) + "\r\n" + _response;
	// _response = head.getHeader(_response.size(), _path, _code, _type, server.getLocations(), server.getLang()) + "\r\n" + _response;
	_response = head.getHeader(_response.size(), _path, _code, _type, server.getLocations(), "\r\n" + _response);


}

void			Response::postMethod(Request & request, Server & server)
{
	ResponseHeader	head;

	if (server.getCgiPass() != "")
	{
		CgiHandler	cgi(request, server);
		size_t		i = 0;
		size_t		j = _response.size() - 2;

		_response = cgi.executeCgi(server.getCgiPass());

		while (_response.find("\r\n\r\n", i) != std::string::npos || _response.find("\r\n", i) == i)
		{
			std::string	str = _response.substr(i, _response.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				_code = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-Type: ") == 0)
				_type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (_response.find("\r\n", j) == j)
			j -= 2;

		_response = _response.substr(i, j - i);
	}
	else
	{
		_code = 204;
		_response = "";
	}
	if (_code == 500)
		_response = this->readHtml(_errorMap[_code]);
	// _response = head.getHeader(_response.size(), _path, _code, _type, server.getLocations(), server.getLang()) + "\r\n" + _response;
	_response = head.getHeader(_response.size(), _path, _code, _type, server.getLocations(), "\r\n" + _response);

}

void			Response::deleteMethod(Request & request, Server & server)
{
	ResponseHeader	head;
	(void)request;

	_response = "";
	if (pathIsFile(_path))
	{
		if (remove(_path.c_str()) == 0)
			_code = 204;
		else
			_code = 403;
	}
	else
		_code = 404;
	if (_code == 403 || _code == 404)
		_response = this->readHtml(_errorMap[_code]);
	_response = head.getHeader(_response.size(), _path, _code, _type, server.getLocations(), "\r\n" + _response;
	// _response = head.getHeader(_response.size(), _path, _code, _type, server.getLocations(), server.getLang()) + "\r\n" + _response;

}

// Utils

int             pathIsFile(const std::string& path)
{
        struct stat s;
        if (stat(path.c_str(), &s) == 0 )
        {
                if (s.st_mode & S_IFDIR)
                        return 0;
                else if (s.st_mode & S_IFREG)
                        return 1;
                else
                        return 0;
        }
        else
                return 0;
}

int				Response::readContent(void)
{
	std::ifstream		file;
	std::stringstream	buffer;

	_response = "";

	if (pathIsFile(_path))
	{
		file.open(_path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			_response = this->readHtml(_errorMap[403]);
			return (403);
		}

		buffer << file.rdbuf();
		_response = buffer.str();

		file.close();
	}
	// else if (_isAutoIndex) {
		// buffer << AutoIndexGenerator::getPage(_path.c_str(),\
			// to_string(_hostPort.host), _hostPort.port);
		// _response = buffer.str();
		// _type = "text/html";
	// }
	else
	{
		_response = this->readHtml(_errorMap[404]);
		return (404);
	}

	return (200);
}

int				Response::writeContent(std::string content)
{
	std::ofstream	file;

	if (pathIsFile(_path))
	{
		file.open(_path.c_str());
		file << content;
		file.close();
		return (204);
	}
	else
	{
		file.open(_path.c_str(), std::ofstream::out | std::ofstream::trunc);
		if (file.is_open() == false)
			return (403);

		file << content;
		file.close();
		return (201);
	}
}

std::string		Response::readHtml(const std::string& path)
{
	std::ofstream		file;
	std::stringstream	buffer;

	if (pathIsFile(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
			return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");

		buffer << file.rdbuf();
		file.close();
		_type = "text/html";

		return (buffer.str());
	}
	else
		return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");
}

// int				Response::fileExists(std::string path) //deprecated, replaced by ::pathIsFile()
// {
// 	struct stat		stats;

// 	if (stat(path.c_str(), &stats) == 0)
// 		return (1);
// 	return (0);
// }

// Getter functions

std::string		Response::getResponse(void) { return (_response); }

// Overloaders

Response & Response::operator=(const Response & src)
{
	_response = src._response;
	_path = src._path;
	_code = src._code;
	return (*this);
}

// Constructors and destructors

Response::Response(void)
{
}

Response::Response(const Response & src)
{
	*this = src;
}

Response::~Response(void)
{
}
