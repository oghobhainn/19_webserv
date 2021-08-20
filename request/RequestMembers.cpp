#include "../main.hpp"

std::ostream &	operator<<(std::ostream& os, const Request & re)
{
	std::map<std::string, std::string>::const_iterator	it;

	os << "Method : " << re.getMethod() << " |\tHTTP version : ";
	os << re.getVersion() << '\n';
	os << "Port : " << re.getPort() << '\n';
	os << "Path : " << re.getPath() << '\n';

	for (it = re.getHeaders().begin(); it != re.getHeaders().end(); it++)
		os << it->first << ": " << it->second << '\n';

	os << '\n' << "Request body :\n" << re.getBody() << '\n';

	return os;
}

void				Request::resetHeaders()
{
	this->_headers.clear();

	this->_headers["Accept-Charsets"] = "";
	this->_headers["Accept-Language"] = "";
	this->_headers["Allow"] = "";
	this->_headers["Auth-Scheme"] = "";
	this->_headers["Authorization"] = "";
	this->_headers["Content-Language"] = "";
	this->_headers["Content-Length"] = "";
	this->_headers["Content-Location"] = "";
	this->_headers["Content-Type"] = "";
	this->_headers["Date"] = "";
	this->_headers["Host"] = "";
	this->_headers["Last-Modified"] = "";
	this->_headers["Location"] = "";
	this->_headers["Referer"] = "";
	this->_headers["Retry-After"] = "";
	this->_headers["Server"] = "";
	this->_headers["Transfer-Encoding"] = "";
	this->_headers["User-Agent"] = "";
	this->_headers["Www-Authenticate"] = "";
	this->_headers["Connection"] = "Keep-Alive";
}

int				Request::readFirstLine(std::string const & str)
{
	size_t		i;
	std::string	line;

	i = str.find_first_of('\n');
	line = str.substr(0, i);
	i = line.find_first_of(' ');

	if (i == std::string::npos)
	{
		this->_ret = 400;
		PE("RFL no space after method");
		return 400;
	}
	this->_method.assign(line, 0, i);
	return this->readPath(line, i);
}

int			Request::readPath(std::string const & line, size_t i)
{
	size_t	j;

	if ((j = line.find_first_not_of(' ', i)) == std::string::npos)
	{
		this->_ret = 400;
		PE("No PATH / HTTP version");
		return 400;
	}
	if (( i = line.find_first_not_of(' ', j)) == std::string::npos)
	{
		this->_ret = 400;
		PE("No HTTP version");
		return 400;
	}
	this->_path.assign(line,j,i-j);
	return this->readVersion(line,i);
}

int		Request::readVersion(std::string const & line, size_t i)
{
	if ((i = line.find_first_not_of(' ', i)) == std::string::npos)
	{
		this->_ret = 400;
		PE("No HTTP version");
		return 400;
	}
	if (line[i] == 'H' && line[i + 1] == 'T' && line[i + 2] == 'T' &&
			line[i + 3] == 'P' && line[i + 4] == '/')
		this->_version.assign(line, i + 5, 3);
	if (this->_version != "1.1")
	{
		this->_ret = 400;
		PE("Bad HTTP version (has to be 1.1)");
		return (this->_ret);
	}
	return (this->checkMethod());
}

int		Request::checkMethod()
{
	for (size_t i = 0; i < this->_method.size(); ++i)
	{
		if (this->methods[i] == this->_method)
			return this->_ret;
	}
	PE("Invalid Method Requested");
	this->_ret = 400;
	return this->_ret;
}

int		Request::checkPort()
{
	size_t i = this->_headers["Host"].find_first_of(':');

	if (i == std::string::npos)
		this->_port = 80;
	else
	{
		std::string tmp(this->_headers["Host"], i + 1);
		this->_port = std::stoi(tmp.c_str());
	}
	return (this->_port);
}

std::string	Request::nextLine(std::string const & str, size_t & i)
{
	std::string	ret;
	size_t		j;

	if (i == std::string::npos)
		return "";
	j = str.find_first_of('\n', i);
	ret = str.substr(i, j-i);
	if (ret[ret.size() - 1] == '\r')
		pop(ret);
	i = (j == std::string::npos ? j : j + 1);
	return ret;
}

int		Request::parse(std::string const & str)
{
	std::string key;
	std::string	value;
	std::string	line;
	size_t		i = 0;

	this->readFirstLine(nextLine(str, i));
	while ((line = nextLine(str, i)) != "\r" && line != "" && this->_ret != 400)
	{
		key = readKey(line);
		value = readValue(line);
		if (this->_headers.count(key))
			this->_headers[key] = value;
		if (key.find("Secret") != std::string::npos)
			this->_cgi_env[formatHeaderForCGI(key)] = value;
	}
	if (this->_headers["Www-Authenticate"] != "")
		this->_cgi_env["Www-Authenticate"] = this->_headers["Www-Authenticate"];
	//this->setLang();
	this->setBody(str.substr(i, std::string::npos));
	this->findQuery();
	return this->_ret;
}

void	Request::stripAll()
{
	strip(this->_method, '\n');
	strip(this->_method, '\r');
	strip(this->_method, ' ');
	strip(this->_version, '\n');
	strip(this->_version, '\r');
	strip(this->_version, ' ');
	strip(this->_path, '\n');
	strip(this->_path, '\r');
	strip(this->_path, ' ');
}

void	Request::findQuery()
{
	size_t	i;

	i = this->_path.find_first_of('?');
	if (i != std::string::npos)
	{
		this->_query.assign(this->_path, i + 1, std::string::npos);
		this->_path = this->_path.substr(0, i);
	}
}

std::string	Request::formatHeaderForCGI(std::string & key)
{
	to_upper(key);
	for (size_t i = 0; i < key.size(); ++i)
	{
		if (key[i] == '-')
			key[i] = '_';
	}
	return "HTTP_" + key;
}