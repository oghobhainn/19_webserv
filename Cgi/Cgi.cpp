#include "Cgi.hpp"

Cgi::Cgi(Request &request, Server &serv): _request(request.getBody())
{
	this->_initEnv(request, serv);
}

Cgi::Cgi(Cgi const &src) 
{
	if (this != &src) 
	{
		this->_request = src._request;
		this->_env = src._env;
	}
}

Cgi::~Cgi(void) 
{
}

Cgi	&Cgi::operator=(Cgi const &src) 
{
	if (this != &src) 
	{
		this->_request = src._request;
		this->_env = src._env;
	}
	return *this;
}

void		Cgi::_initEnv(Request &request, Server &serv) 
{
	std::map<std::string, std::string>	headers = request.getHeaders();

	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		this->_env["AUTH_TYPE"] = headers["Authorization"];
	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SCRIPT_NAME"] = serv.getPath();
	this->_env["SCRIPT_FILENAME"] = serv.getPath();
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_env["CONTENT_LENGTH"] = std::to_string(this->_request.length());
	this->_env["CONTENT_TYPE"] = headers["Content-Type"];
	this->_env["PATH_INFO"] = request.getPath();
	this->_env["PATH_TRANSLATED"] = request.getPath();
	this->_env["QUERY_STRING"] = request.getQuery();
	this->_env["REMOTEaddr"] = serv.getHost();
	this->_env["REMOTE_IDENT"] = headers["Authorization"];
	this->_env["REMOTE_USER"] = headers["Authorization"];
	this->_env["REQUEST_URI"] = request.getPath() + request.getQuery();
	if (headers.find("Hostname") != headers.end())
		this->_env["SERVER_NAME"] = headers["Hostname"];
	else
		this->_env["SERVER_NAME"] = this->_env["REMOTEaddr"];
	this->_env["SERVER_PORT"] = serv.getPort();
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"] = "Webserv/1.0";

	//this->_env.insert(serv.getCgiParam().begin(), serv.getCgiParam().end()); //TODO
}

char	**Cgi::_getEnvAsList() const 
{
	char	**env = new char*[this->_env.size() + 1];
	int	j = 0;

	for (std::map<std::string, std::string>::const_iterator i = this->_env.begin(); i != this->_env.end(); i++) 
	{
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}

std::string		Cgi::handleCgi(const std::string& scriptName) 
{
	pid_t		pid;
	char		**env;
	int			stdin_tmp;
	int			stdout_tmp;
	std::string	new_request;

	try
	{
		env = this->_getEnvAsList();
	}
	catch (std::bad_alloc &e)
	{
		PE(e.what())
	}
	stdin_tmp = dup(STDIN_FILENO);
	stdout_tmp = dup(STDOUT_FILENO);
	FILE	*file_In = tmpfile();
	FILE	*file_Out = tmpfile();
	long	fd_In = fileno(file_In);
	long	fd_Out = fileno(file_Out);
	int		ret = 1;
	write(fd_In, _request.c_str(), _request.size());
	lseek(fd_In, 0, SEEK_SET);
	pid = fork();
	if (pid == 0)
	{
		// char * const * nll = NULL;

		dup2(fd_In, 0);
		dup2(fd_Out, 1);
		execve(scriptName.c_str(), NULL, env);
		PE("Error: Exceve in CGI");
		write(1, "Status: 500\r\n\r\n", 15);
	}
	else if (pid == -1)
	{
		PE("Error: Fork in CGI");
		return ("Status: 500\r\n\r\n");
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(fd_Out, 0, SEEK_SET);
		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fd_Out, buffer, CGI_BUFSIZE - 1);
			new_request += buffer;
		}
	}
	dup2(stdin_tmp, STDIN_FILENO);
	dup2(stdout_tmp, STDOUT_FILENO);
	fclose(file_In);
	fclose(file_Out);
	close(fd_In);
	close(fd_Out);
	close(stdin_tmp);
	close(stdout_tmp);
	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;
	if (!pid)
		exit(0);
	return (new_request);
}

