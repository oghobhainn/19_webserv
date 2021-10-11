#ifndef ACTIVESERVER_HPP
# define ACTIVESERVER_HPP

#include "../ConfigParsing/Config.hpp"
// #include "../Sockets/ListeningSocket.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../Response/ResponseHeader.hpp"
#include <fcntl.h>

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
        long	accept_connection(void);
        void	handle_connection(long socket, std::list<Server> &serv_list);
        // void	processChunk(long socket);
        int		receive_connection(long socket);
        int		send_response(long socket);
        void	close(int socket);
        void	clean(void);

    private:
        std::map<long, std::string>	_requests;
        Server	_listen;
        long    _fd;
        struct sockaddr_in  _addr;

        ActiveServer(void);
};

#endif