#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include "../main.hpp"

class Response{
	public:
		Response();
		Response(Response const & src);
		~Response();
		Response & operator=(Response const & src);

		std::string getResponse();

		void	call(Request & request, RequestConfig & requestConf);
		void	getMethod(Request & request, RequestConfig & requestConf);
		void	postMethod(Request & request, RequestConfig & requestConf);
		void	deleteMethod(Request & request, RequestConfig & requestConf);
		void	optionsMethod(Request & request, RequestConfig & requestConf);
		void	traceMethod(Request & request, RequestConfig & requestConf);

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
		t_listen					_hostPort;
		std::map<int, std::string>	_errorMap;

		static std::map<std::string, void (Response::*)(Request &, RequestConfig &)>	_method;
		static std::map<std::string, void (Response::*)(Request &, RequestConfig &)>	initMethods();
};

#endif