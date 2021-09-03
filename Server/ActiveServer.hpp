#ifndef ACTIVESERVER_HPP
# define ACTIVESERVER_HPP

#include "../ConfigParsing/Config.hpp"

class ActiveServer 
{
    public:
        ActiveServer(Server & listen);
        ActiveServer(const ActiveServer & src);
        ~ActiveServer(void);

        ActiveServer & operator=(const ActiveServer & src);
        long	getFd(void);

        int		setup(void);
        void	setAddr(void);
        // long	accept(void);
        // void	process(long socket, Config & conf);
        // void	processChunk(long socket);
        // int		recv(long socket);
        // int		send(long socket);
        void	close(int socket);
        // void	clean(void);

    private:
        std::map<long, std::string>	_requests;
        Server	_listen;
        long    _fd;
        struct sockaddr_in  _addr;

        ActiveServer(void);
};

#endif