#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <iostream>
#include <list>
#include <map>
#include <vector>

#ifdef DEBUG
# define PRINT 1
#else
# define PRINT 0
#endif

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

#define PE(a) std::cout << RED << a << RESET << std::endl;
#define PY(a) std::cout << YELLOW << a << RESET << std::endl;

/* USAGE :
-	Method and version are stored separtely in a string.
	Any version other than 1.0 or 1.1 is considered invalid
	if the method or the HTTP version is invalid, ret is set to 400 (BAD Request)
	otherwise it stays at its default 200 value
-	Implemented headers are parsed as follows : "KEY:[OWS]VALUE[OWS]"
	and stored in a map<string, string> as (KEY, VALUE)
	OWS = optionnal whitespace
-	An empty line ended by CRLF is considered to be the end of the header section
	anything that follows is stored in a single string labelled "_body"
*/


class Request
{
		private:
			std::string									_method;
			std::string									_version;
			std::map<std::string, std::string>			_headers;
			std::map<std::string, std::string>			_env_for_cgi;
			int											_ret;
			std::string									_body;
			int											_port;
			std::string									_path;
			std::string									_query;
			std::list<std::pair<std::string, float> >	_lang;
			const std::string&							_raw;

			/*** PARSING ***/
			int			readFirstLine(const std::string& line);
			int			readPath(const std::string& line, size_t i);
			int			readVersion(const std::string& line, size_t i);
			int			checkMethod();
			int			checkPort();
			void		findQuery();
			std::string formatHeaderForCGI(std::string& key);
			std::string	nextLine(const std::string &str, size_t& i);
			void		setLang();


			/*** AVAILABLE HTTP METHODS ***/
			static	std::vector<std::string>	methods;

			/*** UNAVAILABLE CTORS ***/
			Request();
			Request(const Request&);

		public:
			Request(const std::string& str);
			~Request();
			Request&	operator=(const Request&);

			/*** GETTERS ***/
			const std::map<std::string, std::string>&			getHeaders() const;
			const std::map<std::string, std::string>&			getEnv() const;
			const std::string&									getMethod() const;
			const std::string&									getVersion() const;
			int													getRet() const;
			const std::string&									getBody() const;
			int													getPort() const;
			const std::string&									getPath() const;
			const std::string&									getQuery() const;
			const std::string&									getRaw() const;
			const std::list<std::pair<std::string, float> >&	getLang() const;

			/*** SETTERS **/
			void	setBody(const std::string& line);
			void	setRet(int);
			void	setMethod(const std::string &method);

			/*** UTILS ****/
			int		parse(const std::string& str);
			void	resetHeaders();
			void	stripAll();

			static std::vector<std::string>		initMethods();

};

std::ostream&	operator<<(std::ostream& os, const Request& re);

int				countSubstr(const std::string& str, const std::string& sub);
int				checkStart(const std::string& str, const std::string& end);
int				checkEnd(const std::string& str, const std::string& end);
std::string		readKey(char *line);
std::string		readKey(const std::string& line);
std::string		readValue(char *line);
std::string		readValue(const std::string& line);
std::string&	pop(std::string& str);
std::string&	strip(std::string& str, char c);
std::string&	to_upper(std::string& str);
std::string&	to_lower(std::string& str);
std::string&	capitalize(std::string& str);
std::string     to_string(size_t n);
bool			compare_langs(const std::pair<std::string, float> first, const std::pair<std::string, float> second);

#endif