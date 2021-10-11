#ifndef CGI_HANDLER_HPP

# define CGI_HANDLER_HPP

# include "../main.hpp"
# include "../Request/Request.hpp"
# include "../Response/Response.hpp"
# include "../Response/ResponseHeader.hpp"

# include <map>

class Cgi {
	public:
		Cgi(Request &request, Server &server); // sets up env according to the request
		Cgi(Cgi const &src);
		virtual ~Cgi(void);

		Cgi   	&operator=(Cgi const &src);
		std::string		handleCgi(const std::string &scriptName);	// executes cgi and returns body
	private:
		Cgi(void);
		void								_initEnv(Request &request, Server &server);
		char								**_getEnvAsList() const;
		int									_getSocket(unsigned int port);
		int									_connectSocket(unsigned int port);
		std::map<std::string, std::string>	_env;
		std::string							_request;
};

#endif

