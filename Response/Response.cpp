#include "Response.hpp"
#include "ResponseHeader.hpp"

std::map<std::string, void (Response::*)(Request &, Server &)>	Response::initMethods()
{
	std::map<std::string, void (Response::*)(Request &, Server &)> map;

	map["GET"] = &Response::getMethod;
	map["POST"] = &Response::postMethod;
	map["DELETE"] = &Response::deleteMethod;
	return map;
}

void			Response::check_method(Request & request, Server & server)
{
	if (server.getGetMethod() == false && request.getMethod() == "GET")
		_code = 405;
	if (server.getPostMethod() == false && request.getMethod() == "POST")
		_code = 405;
	if (server.getDeleteMethod() == false && request.getMethod() == "DELETE")
		_code = 405;
}

void			Response::call(Request & request, Server & server)
{
	std::string tmp_path;
	bool location_found = false;
	bool file_exists = false;

	_error = server.getDefaultErrorPage();
	_code = request.getRet();
	_host = server.getHost();
	_port = server.getPort();
	_path = request.getPath();
	tmp_path = _path;
	// std::cout << "_path begin:" << _path << std::endl; ///////////
	if (_path.size() > 1)
	{
		std::ifstream		file;

		std::string tmp_path = "./default/" + _path;
		file.open(tmp_path.c_str(), std::ifstream::in);
		if (file.is_open() == true || request.getMethod() == "POST")
		{
			_path = "./default/" + _path;
			file_exists = true;
		}
	}
	if (_path.size() == 1 && _path == "/" && file_exists == false)
	{
		if (server.getRoot().size() > 0)
		{
			_path = server.getRoot();
			if (server.getIndex().size() > 0)
				_path = _path + "/" + server.getIndex();
		}
		else
		{
			if (server.getIndex().size() > 0)
				_path = "./default/" + server.getIndex();
			else
				_path = "./default/default.html";
		}
		for (int i = 0; i < server.getNbLoc(); i++)
		{
			if (server.locations[i].extension.size() == 1 && server.locations[i].extension == "/")
			{
				location_found = true;
				if (server.locations[i].get_method == false && request.getMethod() == "GET")
					_code = 405;
				if (server.locations[i].post_method == false && request.getMethod() == "POST")
					_code = 405;
				if (server.locations[i].delete_method == false && request.getMethod() == "DELETE")
					_code = 405;
				if (server.locations[i].root.size() > 0)
				{
					_path = server.locations[i].root + "/";
					if (server.locations[i].index.size() > 0)
						_path = _path + server.locations[i].index;
				}
			}
		}
	}
	else if (file_exists == false)
	{
		if (server.getRoot().size() > 0)
		{
			_path = server.getRoot();
			if (server.getIndex().size() > 0)
				_path = _path + "/" + server.getIndex();
		}
		for (int i = 0; i < server.getNbLoc(); i++)
		{
			if (tmp_path.find(server.locations[i].extension) != std::string::npos && server.locations[i].extension != "/")
			{
				location_found = true;
				if (server.locations[i].get_method == false && request.getMethod() == "GET")
					_code = 405;
				if (server.locations[i].post_method == false && request.getMethod() == "POST")
					_code = 405;
				if (server.locations[i].delete_method == false && request.getMethod() == "DELETE")
					_code = 405;
				if (server.locations[i].root.size() > 0)
				{
					_path = server.locations[i].root + "/";
					if (server.locations[i].index.size() > 0)
						_path = _path + server.locations[i].index;
				}
				else if (server.locations[i].index.size() > 0)
				{
					if (server.getRoot().size() > 1)
						_path = server.getRoot() + "/" + server.locations[i].index;
					else
						_path =  "default/" + server.locations[i].index;
				}
			}
		}
		if (location_found == false && file_exists == false)
		{
			_code = 404;
			_path = "./default/404.html";
		}
	}
	if (location_found == false)
		check_method(request, server);
	if (server.getClientBodySize() < request.getBody().size())
		_code = 413;
	// std::cout << "_path end:" << _path << std::endl; ///////////////
}

void			Response::getMethod(Request & request, Server & server)
{
	ResponseHeader	head;
	
	(void)request;
	if  (_code == 200)
		_code = readContent(server);
	else
	{
		if (server.getDefaultErrorPage().size() > 0)
			_response = this->readHtml("default/" + server.getDefaultErrorPage());
		else
			_response = this->readHtml("default/" + to_string(_code) + ".html");
	}
	if (_code == 500)
	{
		if (server.getDefaultErrorPage().size() > 0)
			_response = this->readHtml("default/" + server.getDefaultErrorPage());
		else
			_response = this->readHtml("default/" + to_string(_code) + ".html");
	}
	std::string body = _response;
	_response = head.getHeader(_response.size(), _path, _code, _type, server.getContentLocation(), "\r\n" + _response);
	_response += "\r\n" + body;
}

void			Response::postMethod(Request & request, Server & server)
{
	ResponseHeader	head;

	if  (_code == 200)
	{
		_code = writeContent(request.getBody());
		_response = "";
	}
	else
	{
		if (server.getDefaultErrorPage().size() > 0)
			_response = this->readHtml("default/" + server.getDefaultErrorPage());
		else
			_response = this->readHtml("default/" + to_string(_code) + ".html");
	}
	if (_code == 500)
		_response = this->readHtml("default/"+ to_string(_code) + ".html");
	_response = head.getHeader(_response.size(), _path, _code, _type, server.getContentLocation(), "\r\n" + _response);
	_response += "\r\n";
}

void			Response::deleteMethod(Request & request, Server & server)
{
	ResponseHeader	head;
	(void)request;
	_response = "";

	if (_code != 200)
	{
		if (server.getDefaultErrorPage().size() > 0)
			_response = this->readHtml("default/" + server.getDefaultErrorPage());
		else
			_response = this->readHtml("default/" + to_string(_code) + ".html");
	}
	else if (pathIsFile(_path))
	{
		if (remove(_path.c_str()) == 0)
		{
			PY(_path + " DELETED");
			_code = 204;
		}
		else
			_code = 403;
	}
	else
		_code = 404;
	if (_code == 403 || _code == 404)
		_response = this->readHtml("default"+ to_string(_code) + ".html");
	_response = head.getHeader(_response.size(), _path, _code, _type, server.getContentLocation(), "\r\n" + _response);
	_response += "\r\n";
}

Response		Response::buildResponse(Request & req, Server & serv)
{
	std::cout << "get_methode == " << req.getMethod() << std::endl;
	if (req.getMethod() == "GET")
		Response::getMethod(req, serv);
	if (req.getMethod() == "POST")
		Response::postMethod(req, serv);
	if (req.getMethod() == "DELETE")
		Response::deleteMethod(req, serv);
	return	*this;
}

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

int				Response::readContent(Server & server)
{
	std::ifstream		file;
	std::stringstream	buffer;

	_response = "";
	if (pathIsFile(_path))
	{
		file.open(_path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			if (server.getDefaultErrorPage().size() > 0 && pathIsFile("default/" + server.getDefaultErrorPage()))
			{
				_response = this->readHtml("default/" + server.getDefaultErrorPage());
				return(403);
			}
			_response = this->readHtml("default/403.html");
			return (403);
		}
		buffer << file.rdbuf();
		_response = buffer.str();
		file.close();
	}
	else
	{
		if (server.getDefaultErrorPage().size() > 0 && pathIsFile("default/" + server.getDefaultErrorPage()))
		{
			_response = this->readHtml("default/" + server.getDefaultErrorPage());
			return(404);
		}
		_response = this->readHtml("default/404.html");
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
	std::cout << "path = " << path << std::endl;
	if (pathIsFile(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
			return ("<!DOCTYPE html>\n<html><title>404</title><body>Error 404: Ressource not found</body></html>\n");
		buffer << file.rdbuf();
		file.close();
		_type = "text/html";
		return (buffer.str());
	}
	else
		return ("<!DOCTYPE html>\n<html><title>404</title><body>Error 404: Ressource not found</body></html>\n");
}

std::string		Response::getResponse(void) { return (_response); }

Response & Response::operator=(const Response & src)
{
	_response = src._response;
	_path = src._path;
	_code = src._code;
	return (*this);
}

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



