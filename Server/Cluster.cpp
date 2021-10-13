#include "Cluster.hpp"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*p;
	unsigned char	*q;

	i = 0;
	p = (unsigned char *)dst;
	q = (unsigned char *)src;
	while (i < n)
	{
		p[i] = q[i];
		i++;
	}
	return (dst);
}

Cluster::Cluster(std::list<Server> serv_list)
{
    _serv_list = serv_list;
}

Cluster::Cluster(const Cluster & src)
{
	if (this != &src)
		*this = src;
}

Cluster::~Cluster(void)
{
}

int		Cluster::setup(void)
{
	FD_ZERO(&_fd_set);
	_max_fd = 0;
    unsigned int nb_server = 0;

    for (std::list<Server>::iterator it = _serv_list.begin(); it != _serv_list.end(); ++it)
        nb_server++;
	_fd_size = nb_server;
	for (std::list<Server>::iterator it2 = _serv_list.begin(); it2 != _serv_list.end(); ++it2)
	{
	    ActiveServer		serv(*it2);
		long		        fd;

		if (serv.setup() != -1)
		{
			fd = serv.getFd();
			FD_SET(fd, &_fd_set);
			_servers.insert(std::make_pair(fd, serv));
			if (fd > _max_fd)
				_max_fd = fd;
			std::cout << it2->getHost() << ":" << it2->getPort() << " is setup" << std::endl;
		}
		else 
			return (-1);
	}
    if (_max_fd == 0)
    {
        std::cerr << "Could not setup cluster " << std::endl;
        return (-1);
    }
    return (0);
}

int		Cluster::sendResponse(int ret, fd_set &reading_set, fd_set &writing_set)
{
	for (std::vector<int>::iterator it = _ready.begin(); ret && it != _ready.end(); it++)
	{
		if (FD_ISSET(*it, &writing_set))
		{
			long	ret = _sockets[*it]->send_response(*it);

			if (ret == 0)
				_ready.erase(it);
			else if (ret == -1)
			{
				FD_CLR(*it, &_fd_set);
				FD_CLR(*it, &reading_set);
				_sockets.erase(*it);
				_ready.erase(it);
			}
			break;
		}
	}
	return (ret);
}

int		Cluster::readRequest(int ret, fd_set &reading_set)
{
	for (std::map<long, ActiveServer *>::iterator it = _sockets.begin(); ret && it != _sockets.end(); it++)
	{
		long	socket = it->first;

		if (FD_ISSET(socket, &reading_set))
		{
			long	ret = it->second->receive_connection(socket);
			if (ret == 0)
			{
				it->second->handle_connection(socket, _serv_list);
				_ready.push_back(socket);
				
			}
			else if (ret == -1)
			{
				FD_CLR(socket, &_fd_set);
				FD_CLR(socket, &reading_set);
				_sockets.erase(socket);
				it = _sockets.begin();
			}
			ret = 0;
			break;
		}
	}
	return (ret);
}

int		Cluster::acceptConnection(int ret, fd_set &reading_set)
{
	for (std::map<long, ActiveServer>::iterator it = _servers.begin(); ret && it != _servers.end(); it++)
	{
		long	fd = it->first;

		if (FD_ISSET(fd, &reading_set))
		{
			long	socket = it->second.accept_connection();
			if (socket != -1)
			{
				FD_SET(socket, &_fd_set);
				_sockets.insert(std::make_pair(socket, &(it->second)));
				if (socket > _max_fd)
					_max_fd = socket;
			}
			ret = 0;
			break;
		}
	}
	return (ret);
}

void	Cluster::run(void)
{
	while (true)
	{
		fd_set		reading_set;
		fd_set		writing_set;
		struct timeval timeout = {2, 0};
		int				ret = 0;

		std::cout << "..." << std::endl;
		while (ret == 0)
		{
			ft_memcpy(&reading_set, &_fd_set, sizeof(_fd_set));
			FD_ZERO(&writing_set);
			for (std::vector<int>::iterator it = _ready.begin() ; it != _ready.end() ; it++)
				FD_SET(*it, &writing_set);
			ret = select(_max_fd + 1, &reading_set, &writing_set, NULL, &timeout);
		}
		// Si le select a trouvé une socket active
		if (ret > 0)
		{
		 	ret = sendResponse(ret, reading_set, writing_set);
			ret = readRequest(ret, reading_set);
			ret = acceptConnection(ret, reading_set);
		}
		// si le select a foiré
		else
		{
			std::cerr << "Problem with select" << std::endl;
			for (std::map<long, ActiveServer *>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
				it->second->close_socket(it->first);
			_sockets.clear();
			_ready.clear();
			FD_ZERO(&_fd_set);
			for (std::map<long, ActiveServer>::iterator it = _servers.begin(); it != _servers.end(); it++)
				FD_SET(it->first, &_fd_set);
		}
	}
}

void	Cluster::clean(void)
{
	for ( std::map<long, ActiveServer>::iterator it = _servers.begin(); it != _servers.end(); it++)
		it->second.clean_server();
}

Cluster & Cluster::operator=(const Cluster & src)
{
	_fd_set = src._fd_set;
	_fd_size = src._fd_size;
	_max_fd = src._max_fd;
	return (*this);
}
