#include "Response.hpp"
#include "ResponseHeader.hpp"

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
	bool location_found = false; // si il rentre dans une loc, devient true
	bool default_root = false; // pour gerer si path = "/" et eviter que le .find le trouve tout le temps
	// _errorMap = server.getErrorPage();
	// _isAutoIndex = server.getAutoIndex(); //TODO
	// _isAutoIndex = server.getAutoIndex();
	_error = server.getDefaultErrorPage();
	_code = request.getRet();
	_host = server.getHost();
	_port = server.getPort();
	_path = request.getPath();
	std::cout << "_path begin:" << _path << std::endl;
	// Case: root at the beginning
	if (server.getRoot().size() > 0)
	{
		_path = server.getRoot();
		// Case: index at the beginning
		if (server.getIndex().size() > 0)
			_path = _path + "/" + server.getIndex();
	}
	// Case: normal path without root
	else if (_path.size() == 1 && _path == "/")
	{
		default_root = true;
		_path = "./default/default.html";
		// Case: index at the beginning
		if (server.getIndex().size() > 0)
			_path = "./default/" + server.getIndex();
	}
	// Case: upload a file
	else if (_path == "/upload.html")
		_path = "./default/upload.html";
	// Case: delete ?
	else if (_path == "/delete/example.html")
		_path = "DELETE /frontend/example.html HTTP/1.1";
	// Case: check if there is a loc in the url
	std::cout << "_path begin:" << _path << std::endl;
	for (int i = 0; i < server.getNbLoc(); i++)
	{
		std::cout << "++++++++++++++++++++++" << std::endl;
		std::cout << "loc:" << server.locations[i].extension << std::endl;
		std::cout << "path:" << _path << std::endl;
		std::cout << "size = " << server.locations[i].extension.size() << std::endl;
		if (default_root == true && server.locations[i].extension.size() == 1 && server.locations[i].extension == "/")
		{
			std::cout << "=========================================== !!found!! ===========================================" << std::endl;
			location_found = true;
			if (server.locations[i].root.size() > 0)
			{
				_path = server.locations[i].root + "/";
				if (server.locations[i].index.size() > 0)
					_path = _path + server.locations[i].index;
			}
		}
		else if (_path.find(server.locations[i].extension) != std::string::npos && server.locations[i].extension != "/")
		{
			std::cout << "innnnn location" << std::endl;
			location_found = true;
			// A METTRE ICI TOUTES LES CONTRAINTES LIEES AUX LOCS :
			if (server.locations[i].get_method == false && request.getMethod() == "GET")
				_code = 405;
			if (server.locations[i].post_method == false && request.getMethod() == "POST")
				_code = 405;
			if (server.locations[i].delete_method == false && request.getMethod() == "DELETE")
				_code = 405;
			// A VOIR: changer le dossier ou sera uploadé le fichier 
			//if (request.getMethod() == "POST" && server.locations[i].file_upload_location != "./default/")
			if (server.locations[i].root.size() > 0)
			{
				_path = server.locations[i].root + "/";
				if (server.locations[i].index.size() > 0)
					_path = _path + server.locations[i].index;
			}
		}
		// std::cout << "---------------------" << std::endl;
	}
	// Case: wrong url / pas encore la bonne methode de faire comme ca
	

	// necessaire? car deja le cas ...
	if (location_found == false && default_root == false)
	{
		_code = 404;
		std::cout << "Wrong URL" << std::endl;
		_path = "./default/404.html";
		// return ; // necessaire?
	}

	std::cout << "_path end:" << _path << std::endl;

	std::set<std::string> allowedTODO;
	allowedTODO.insert("GET");
	allowedTODO.insert("POST");
	allowedTODO.insert("DELETE");

	if (allowedTODO.find(request.getMethod()) == allowedTODO.end())
		_code = 405;
	else if (server.getClientBodySize() < request.getBody().size())
		_code = 413;
	if (_code == 405 || _code == 413)
	{
		ResponseHeader	head;
		// _response = head.notAllowed(server.getAllowedMethods(), server.getLocations(), _code, server.getLang() + "\r\n";
		_response = head.notAllowed(server.getAllowedMethods(), server.getContentLocation(), _code, "\r\n");
		return ;
	}
	// if (server.getAllowedMethods().find(request.getMethod()) == server.getAllowedMethods().end())
	// {
	// 	_code = 405;
	// }
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
	// _response = head.getHeader(_response.size(), _path, _code, _type, server.getContentLocation(), server.getLang()) + "\r\n" + _response;
	// _response = head.getHeader(_response.size(), _path, _code, _type, server.getLocations(), server.getLang()) + "\r\n" + _response;
	std::string body = _response;
	_response = head.getHeader(_response.size(), _path, _code, _type, server.getContentLocation(), "\r\n" + _response);
	_response += "\r\n" + body;
}

void			Response::postMethod(Request & request, Server & server)
{
	ResponseHeader	head;

	if (server.getCgiPass() != "")
	{
		CgiHandler	cgi(request, server);
		size_t		i = 0;
		size_t		j = _response.size() - 2;

		_response = cgi.executeCgi(server.getCgiPass()); //magic happens here TODO

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
		_code = writeContent(request.getBody());
		_response = "";
	}
	if (_code == 500)
		_response = this->readHtml("html/error"+ to_string(_code) + ".html");
	// _response = head.getHeader(_response.size(), _path, _code, _type, server.getLocations(), server.getLang()) + "\r\n" + _response;
	_response = head.getHeader(_response.size(), _path, _code, _type, server.getContentLocation(), "\r\n" + _response);
	_response += "\r\n";
}

// Un code de statut 202 (Accepted) si l'action est en passe de réussir mais n'a pas encore été confirmée.
// Un code de statut 204 (No Content) si l'action a été confirmée et qu'aucune information supplémentaire n'est à fournir.
// Un code de statut 200 (OK) si l'action a été confirmée et que le message de réponse inclut une représentation décrivant le statut.
// <html>
//   <body>
	// <h1>File deleted.</h1>
//   </body>
// </html>
	
void			Response::deleteMethod(Request & request, Server & server)
{
	ResponseHeader	head;
	(void)request;
	_response = "";

	if (pathIsFile(_path))
	{
		std::cout << "---------------------------------------------------------------- ok --------------------------" << std::endl;
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
		_response = this->readHtml("html/error"+ to_string(_code) + ".html");
	_response = head.getHeader(_response.size(), _path, _code, _type, server.getContentLocation(), "\r\n" + _response);
	// _response = head.getHeader(_response.size(), _path, _code, _type, server.getLocations(), server.getLang()) + "\r\n" + _response;
	_response += "\r\n";
}

Response		Response::buildResponse(Request & req, Server & serv)
{
	// PY("Allowed Methods: ");
	// for(std::set<std::string>::iterator it = serv.getAllowedMethods().begin(); it != serv.getAllowedMethods().end(); it++)
    // {
    //     PY(*it);
    // }
	std::cout << "get_methode == " << req.getMethod() << std::endl;
	if (req.getMethod() == "GET") //if its allowed, TODO : gotta use setMethodsAllowed in the parsing of .conf 
		Response::getMethod(req, serv);
	if (req.getMethod() == "POST")
		Response::postMethod(req, serv);
	if (req.getMethod() == "DELETE")
		Response::deleteMethod(req, serv);
	return	*this;
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
			_response = this->readHtml("html/error/403.html");
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
		_response = this->readHtml("html/error/404.html");
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
			return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");

		buffer << file.rdbuf();
		file.close();
		_type = "text/html";

		return (buffer.str());
	}
	else
		return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");
}

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

