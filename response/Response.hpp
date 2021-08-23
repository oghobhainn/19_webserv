#ifndef RESPONSE_HPP
#define RESPONSE_HPP

//response hpp

# include "../main.hpp"

class Response{
	public:
		Response();
		Response(Response const & src);
		~Response();
		Response & operator=(Response const & src);

		std::string getResponse();

		void	call(Request & request, Server & serv);
		void	getMethod(Request & request, Server & serv);
		void	postMethod(Request & request, Server & serv);
		void	deleteMethod(Request & request, Server & serv);
		void	optionsMethod(Request & request, Server & serv);
		void	traceMethod(Request & request, Server & serv);

		int			readContent();
		int			writeContent(std::string content);
		int			fileExists(std::string path);
		std::string	readHtml(std::string const & path);

	private:
		std::string					_response;
		std::string					_path;
		int							_code;
		std::string					_type;
		bool						_isAutoIndex;
		std::string					_host;
		std::string					_port;
		std::map<int, std::string>	_errorMap;

		static std::map<std::string, void (Response::*)(Request &, Server &)>	_method;
		static std::map<std::string, void (Response::*)(Request &, Server &)>	initMethods();
};

#endif