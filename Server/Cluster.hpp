#ifndef CLUSTER_HPP
# define CLUSTER_HPP


# include "ActiveServer.hpp"
# include "../ConfigParsing/Config.hpp"



class Cluster 
{
    public:
        Cluster(void);
        Cluster(const Cluster & src);
        Cluster(std::list<Server> serv_list);
        ~Cluster(void);
        Cluster & operator=(const Cluster & src);

        int		setup(void);
        int     sendResponse(int ret, fd_set &reading_set, fd_set &writing_set);
        int     readRequest(int ret, fd_set &reading_set);
        int     acceptConnection(int ret, fd_set &reading_set);
        void	run(void);
        void	clean(void);

    private:
        std::list<Server>			        _serv_list;
        std::map<long, ActiveServer>		_servers;
        std::map<long, ActiveServer *>	    _sockets;
        std::vector<int>			        _ready;
        fd_set						_fd_set;
        unsigned int				_fd_size;
        long						_max_fd;
};

#endif