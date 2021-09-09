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

// Constructors and destructors

Cluster::Cluster(std::list<Server> serv_list)
{
    _serv_list = serv_list;
}

Cluster::Cluster(const Cluster & src)
{
	if (this != &src) {
		*this = src;
	}
}

Cluster::~Cluster(void)
{
}

// Member functions

// void	Cluster::config(std::string fileconf)
// {
// 	_config.parse(fileconf.c_str());
// }

int		Cluster::setup(void)
{
	FD_ZERO(&_fd_set);
	_max_fd = 0;
    unsigned int nb_server = 0;
	// int *ports_lst = NULL;
	// int i = 0;
	// int port;

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

void	Cluster::run(void)
{
	int	n = 0;

	while (true)
	{
		fd_set		reading_set;
		fd_set		writing_set;
		struct timeval timeout = {2, 0};
		int				ret = 0;

		// boucle du select: selectione les sockets actives.
		while (ret == 0)
		{
			ft_memcpy(&reading_set, &_fd_set, sizeof(_fd_set));
			FD_ZERO(&writing_set);
			for (std::vector<int>::iterator it = _ready.begin() ; it != _ready.end() ; it++)
			{
				FD_SET(*it, &writing_set);
				std::cout << "---------- add socket writing : " << *it << std::endl;
			}
			std::cout << "Waiting on a connection" << std::endl;
			if (n == 3)
				n = 0;
			ret = select(_max_fd + 1, &reading_set, &writing_set, NULL, &timeout);
		}
		// if ret > 0 : select a trouvé une socket active
		if (ret > 0)
		{
			// boucle du write: envoie les requetes des sockets clients.
		 	for (std::vector<int>::iterator it = _ready.begin(); ret && it != _ready.end(); it++)
		 	{
		 		if (FD_ISSET(*it, &writing_set))
		 		{
					std::cout << "---------- start writing : " << *it << std::endl;
					long	ret = _sockets[*it]->send(*it);

					if (ret == 0)
						_ready.erase(it);
					else if (ret == -1)
					{
						FD_CLR(*it, &_fd_set);
						FD_CLR(*it, &reading_set);
						_sockets.erase(*it);
						_ready.erase(it);
					}
					// ret = 0;
					break;
		 		}
		 	}
			// lit les informations de la requetes
		 	for (std::map<long, ActiveServer *>::iterator it = _sockets.begin(); ret && it != _sockets.end(); it++)
		 	{
		 		long	socket = it->first;

		 		if (FD_ISSET(socket, &reading_set))
				{
					std::cout << "---------- read socket : " << socket << std::endl;
		 			long	ret = it->second->recv(socket);

		 			if (ret == 0)
		 			{
		 				it->second->process(socket, _serv_list);
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
			// lorsqu'une serveur s'active, on cree une socket cliente pour lire par la suite la requete
			for (std::map<long, ActiveServer>::iterator it = _servers.begin(); ret && it != _servers.end(); it++)
			{
			 	long	fd = it->first;

			 	if (FD_ISSET(fd, &reading_set))
			 	{
			 		long	socket = it->second.accept();
					std::cout << "---------- socket created : " << socket << " for the port : " << fd << std::endl;
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
		}
		// si le select foire
		else
		{
			std::cerr << "Problem with select" << std::endl;
			for (std::map<long, ActiveServer *>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
				it->second->close(it->first);
			_sockets.clear();
			_ready.clear();
			FD_ZERO(&_fd_set);
			for (std::map<long, ActiveServer>::iterator it = _servers.begin(); it != _servers.end(); it++)
				FD_SET(it->first, &_fd_set);
		}
		std::cout << "-- END --" << std::endl;
		n = 0;
	}
}

void	Cluster::clean(void)
{
	for ( std::map<long, ActiveServer>::iterator it = _servers.begin(); it != _servers.end(); it++)
		it->second.clean();
}

// // Overloaders

Cluster & Cluster::operator=(const Cluster & src)
{
	// _config = src._config;
	// _servers = src._servers;
	_fd_set = src._fd_set;
	_fd_size = src._fd_size;
	_max_fd = src._max_fd;
	return (*this);
}
