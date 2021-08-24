#ifndef CGI_HANDLER_HPP

# define CGI_HANDLER_HPP

# include "../main.hpp"
# include "../Request/Request.hpp"
# include "../Response/Response.hpp"
# include "../Response/ResponseHeader.hpp"

# include <map>

class CgiHandler {
	public:
		CgiHandler(Request &request, Server &server); // sets up env according to the request
		CgiHandler(CgiHandler const &src);
		virtual ~CgiHandler(void);

		CgiHandler   	&operator=(CgiHandler const &src);
		std::string		executeCgi(const std::string &scriptName);	// executes cgi and returns body
	private:
		CgiHandler(void);
		void								_initEnv(Request &request, Server &server);
		char								**_getEnvAsCstrArray() const;
		int									_getSocket(unsigned int port);
		int									_connectSocket(unsigned int port);
		std::map<std::string, std::string>	_env;
		std::string							_body;
};

#endif

