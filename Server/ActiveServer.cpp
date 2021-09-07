#include "ActiveServer.hpp"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*p;

	i = 0;
	p = (unsigned char *)b;
	while (i < len)
		p[i++] = c;
	return (b);
}

ActiveServer::ActiveServer(void)
{
}

ActiveServer::ActiveServer(Server & listen)
{
	_listen = listen;
	_fd = -1;
	this->setAddr();
}

ActiveServer::ActiveServer(const ActiveServer & src)
{
	*this = src;
}

ActiveServer::~ActiveServer(void)
{
}

int		ActiveServer::setup(void)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
	{
		std::cerr << "Could not create server." << std::endl;
		return (-1);
	}
	if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
	{
		std::cerr << "Could not bind port " << _listen.getPort() << "." << std::endl;
		return (-1);
	}
	if (listen(_fd, 1000) == -1)
	{
		std::cerr << "Could not listen." << std::endl;
		return (-1);
	}
	return (0);
}

void ActiveServer::setAddr(void)
{
	// ft_memset((char *)&_addr, 0, sizeof(_addr));

    // int host = stoi(_listen.getHost());
    int port = stoi(_listen.getPort());

	_addr.sin_family = AF_INET;
	// INADDR_ANY : 0.0.0.0
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_addr.sin_port = htons(port);
}

long		ActiveServer::accept(void)
{
	long	socket;

	socket = ::accept(_fd, NULL, NULL);
	if (socket == -1)
		std::cerr << RED << "Could not create socket. " << strerror(errno) << std::endl;
	else
	{
		fcntl(socket, F_SETFL, O_NONBLOCK);
		_requests.insert(std::make_pair(socket, ""));
	}
	return (socket);
}

void		ActiveServer::process(long socket, std::list<Server> &serv_list)
{
	std::list<Server> test;
// 	RequestConfig	requestConf;
// 	Response		response;
	std::string		recvd = "";
	test = serv_list;

// 	if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos &&
// 		_requests[socket].find("Transfer-Encoding: chunked") < _requests[socket].find("\r\n\r\n"))
// 		this->processChunk(socket);

// 	if (OUT)
// 	{
// 		if (_requests[socket].size() < 1000)
// 			std::cout << "\nRequest :" << std::endl << "[" << YELLOW << _requests[socket] << RESET << "]" << std::endl;
// 		else
// 			std::cout << "\nRequest :" << std::endl << "[" << YELLOW << _requests[socket].substr(0, 1000) << "..." << _requests[socket].substr(_requests[socket].size() - 10, 15) << RESET << "]" << std::endl;
// 	}
 	if (_requests[socket] != "")
 	{
// 		Request			request(_requests[socket]);
		Request     req(_requests[socket], _listen);

		std::cout << "=============================================================================================" << std::endl;
		PY("request : ");
		std::cout << req << std::endl;
		// std::cout << _buffer << std::endl;

		Response		response;
		response.call(req, _listen);
		response = response.Response::buildResponse(req, _listen);

		std::cout << "=============================================================================================" << std::endl;
		PY("response : ");
		std::cout << response.getResponse() << std::endl;
		char char_response[response.getResponse().length() + 1];
		strcpy(char_response, response.getResponse().c_str()); 
		write(socket, char_response, strlen(char_response));
		// close(socket);

// 		if (request.getRet() != 200)
// 			request.setMethod("GET");

// 		requestConf = conf.getConfigForRequest(this->_listen,  request.getPath(), request.getHeaders().at("Host"), request.getMethod(), request);

// 		response.call(request, requestConf);

		_requests.erase(socket);
 		_requests.insert(std::make_pair(socket, response.getResponse()));
 	}
}

// void		Server::processChunk(long socket)
// {
// 	std::string	head = _requests[socket].substr(0, _requests[socket].find("\r\n\r\n"));
// 	std::string	chunks = _requests[socket].substr(_requests[socket].find("\r\n\r\n") + 4, _requests[socket].size() - 1);
// 	std::string	subchunk = chunks.substr(0, 100);
// 	std::string	body = "";
// 	int			chunksize = strtol(subchunk.c_str(), NULL, 16);
// 	size_t		i = 0;

// 	while (chunksize)
// 	{
// 		i = chunks.find("\r\n", i) + 2;
// 		body += chunks.substr(i, chunksize);
// 		i += chunksize + 2;
// 		subchunk = chunks.substr(i, 100);
// 		chunksize = strtol(subchunk.c_str(), NULL, 16);
// 	}

// 	_requests[socket] = head + "\r\n\r\n" + body + "\r\n\r\n";
// }

int			ActiveServer::recv(long socket)
{
 	char	buffer[10000] = {0};
 	int		ret;

 	ret = ::recv(socket, buffer, 10000, 0);

 	if (ret == 0 || ret == -1)
 	{
 		this->close(socket);
 		if (!ret)
 			std::cout << "Connection closed by client" << std::endl;
 		else
 			std::cout << "Read error, closing connection" << std::endl;
 		return (-1);
 	}
 	_requests[socket] += std::string(buffer);
	//std::cout << _requests[socket] << std::endl;

 	size_t	i = _requests[socket].find("\r\n\r\n");
 	if (i != std::string::npos)
 	{
 		if (_requests[socket].find("Content-Length: ") == std::string::npos)
 		{
			if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos)
			{
				if (checkEnd(_requests[socket], "0\r\n\r\n") == 0)
					return (0);
				else
					return (1);
			}
			else
 				return (0);
 		}

 		// size_t	len = std::atoi(_requests[socket].substr(_requests[socket].find("Content-Length: ") + 16, 10).c_str());
// 		if (_requests[socket].size() >= len + i + 4)
// 			return (0);
// 		else
// 			return (1);
 	}
 	return (1);
}

int			ActiveServer::send(long socket)
{
	static std::map<long, size_t>	sent;

	if (sent.find(socket) == sent.end())
		sent[socket] = 0;

// 	if (OUT && sent[socket] == 0)
// 	{
// 		if (_requests[socket].size() < 1000)
// 			std::cout << "\rResponse :                " << std::endl << "[" << GREEN << _requests[socket] << RESET << "]\n" << std::endl;
// 		else
// 			std::cout << "\rResponse :                " << std::endl << "[" << GREEN << _requests[socket].substr(0, 1000) << "..." << _requests[socket].substr(_requests[socket].size() - 10, 15) << RESET << "]\n" << std::endl;
	// }
	std::string	str = _requests[socket].substr(sent[socket], 10000);
 	int	ret = ::send(socket, str.c_str(), str.size(), 0);
	if (ret == -1)
	{
		this->close(socket);
		sent[socket] = 0;
		return (-1);
	}
	else
	{
		sent[socket] += ret;
		if (sent[socket] >= _requests[socket].size())
		{
			_requests.erase(socket);
			sent[socket] = 0;
			return (0);
		}
		else
			return (1);
	}
}

void ActiveServer::close(int socket)
{
	if (socket > 0)
		::close(socket);
	_requests.erase(socket);
}

void		ActiveServer::clean(void)
{
	if (_fd > 0)
		::close(_fd);
	_fd = -1;
}

long ActiveServer::getFd(void)
{
	return (_fd);
}

ActiveServer &	ActiveServer::operator=(const ActiveServer & src)
{
	_listen = src._listen;
	_fd = src._fd;
	_addr = src._addr;
	return (*this);
}