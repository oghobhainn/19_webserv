#include "CgiHandler.hpp"

CgiHandler::CgiHandler(Request &request, Server &serv): _body(request.getBody())
{
	this->_initEnv(request, serv);
}

CgiHandler::CgiHandler(CgiHandler const &src) 
{
	if (this != &src) 
	{
		this->_body = src._body;
		this->_env = src._env;
	}
}

CgiHandler::~CgiHandler(void) 
{
}

CgiHandler	&CgiHandler::operator=(CgiHandler const &src) 
{
	if (this != &src) 
	{
		this->_body = src._body;
		this->_env = src._env;
	}
	return *this;
}

void		CgiHandler::_initEnv(Request &request, Server &serv) 
{
	std::map<std::string, std::string>	headers = request.getHeaders();

	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		this->_env["AUTH_TYPE"] = headers["Authorization"];
	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SCRIPT_NAME"] = serv.getPath();
	this->_env["SCRIPT_FILENAME"] = serv.getPath();
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_env["CONTENT_LENGTH"] = std::to_string(this->_body.length());
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

char	**CgiHandler::_getEnvAsCstrArray() const 
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

std::string		CgiHandler::executeCgi(const std::string& scriptName) 
{
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;
	char		**env;
	std::string	newBody;

	try {
		env = this->_getEnvAsCstrArray();
	}
	catch (std::bad_alloc &e) {
		PE(e.what())
	}
	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);
	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;
	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);
	pid = fork();
	if (pid == -1)
	{
		PE("Fork didn't work");
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid)
	{
		char * const * nll = NULL;

		dup2(fdIn, 0);
		dup2(fdOut, 1);
		execve(scriptName.c_str(), nll, env);
		PE("Execve didn't work");
		write(1, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fdOut, buffer, CGI_BUFSIZE - 1);
			newBody += buffer;
		}
	}
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);
	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;
	if (!pid)
		exit(0);
	return (newBody);
}

