#include "../main.hpp"

class Request
{

	public:
		Request(const std::string & str);
		~Request();
		Request &	operator=(const Request &);

		const std::map<std::string, std::string>&		getHeaders() const;
		const std::map<std::string, std::string>&		getEnv() const;
		const std::string&								getMethod() const;
		const std::string&								getVersion() const;	
		int												getRet() const;
		const std::string&								getBody() const;
		int												getPort() const;
		const std::string&								getPath() const;
		const std::string&								getQuery() const;
		const std::string&								getRaw() const;
		const std::list<std::pair<std::string, float> >&	getLang() const;

		void	setBody(const std::string &line);
		void	setRet(int);
		void	setMethod(const std::string &method);

		int									parse(const std::string& str);
		void								resetHeaders();
		void								stripAll();

		static std::vector<std::string>		initMethods();

	private:
		std::string									_method;
		std::string									_version;
		std::map<std::string, std::string>			_headers;
		std::map<std::string, std::string>			_cgi_env;
		int											_ret;
		std::string									_body;
		int											_port;
		std::string									_path;
		std::string									_query;
	//	std::list< std::pair<std::string, float> >	_lang;
		const std::string							_raw;


		/* PARSING */
		int			readFirstLine(std::string const & line);
		int			readPath(std::string const & ine, size_t i);
		int			readVersion(std::string const & line, size_t i);
		int			checkMethod();
		int			checkPort();
		void		findQuery();
		std::string	formatHeaderForCGI(std::string & key);
		std::string	nextLine(std::string const & str, size_t & i);
		void		setLang();

		/* AVAILABLE HTTP METHODS */
		static		std::vector<std::string>	methods;

		/* UNAVAILABLE CTORS */
		Request();
		Request(const Request&);
};

std::ostream&	operator<<(std::ostream& os, const Request& re);
