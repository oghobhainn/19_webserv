#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../Request/Request.hpp"
# include "../ConfigParsing/Config.hpp"

class Response {
public:
	Response(void);
	Response(const Response & src);
	~Response(void);

	Response & operator=(const Response & src);

	// Getter functions
	std::string		getResponse(void);

	// Member functions
	// void			call(Request & request, RequestConfig & requestConf);

	// void			getMethod(Request & request, RequestConfig & requestConf);
	// void			postMethod(Request & request, RequestConfig & requestConf);
	// void			deleteMethod(Request & request, RequestConfig & requestConf);

	void			call(Request & request, Server & server);

	void			check_method(Request & request, Server & server);

	void			getMethod(Request & request, Server & server);
	void			postMethod(Request & request, Server & server);
	void			deleteMethod(Request & request, Server & server);

	int				readContent(Server & server);
	int				writeContent(std::string content);
	int				fileExists(std::string path);
	std::string		readHtml(const std::string& path);

	Response		buildResponse(Request & req, Server & serv);

private:
	std::string					_response;
	std::string					_path;
	int							_code;
	std::string					_type;
	bool						_isAutoIndex;
	std::string					_host; //henry's Server class
	std::string					_port; // ""
	// t_listen					_hostPort;
	std::string					_error;
	std::map<int, std::string>	_errorMap;

	// static std::map<std::string, void (Response::*)(Request &, RequestConfig &)>	_method;
	// static std::map<std::string, void (Response::*)(Request &, RequestConfig &)>	initMethods();

	static std::map<std::string, void (Response::*)(Request &, Server &)>	_method;
	static std::map<std::string, void (Response::*)(Request &, Server &)>	initMethods();
};

int             pathIsFile(const std::string& path);

#endif