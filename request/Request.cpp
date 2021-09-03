#include "Request.hpp"

std::vector<std::string>		Request::initMethods()
{
	std::vector<std::string>	methods;

	methods.push_back("GET");
	methods.push_back("POST");
	methods.push_back("DELETE");

	return methods;
}

std::vector<std::string>	Request::methods = Request::initMethods();

Request::Request(const std::string& str, Server serv) :
	_method (""), _version(""), _ret(200), _body(""), _port(0), _path(""), _query(""), _raw(str)
{
	this->resetHeaders();
	this->_env_for_cgi.clear();
	this->parse(str);
	_port = stoi(serv.getPort());

	if (this->_ret != 200)
		PE("Parse error : " + std::to_string(this->_ret));
}

Request::~Request()
{
}

Request&	Request::operator=(const Request& obj)
{
	this->_headers = obj.getHeaders();
	this->_env_for_cgi = obj.getEnv();
	this->_method = obj.getMethod();
	this->_version = obj.getVersion();
	this->_ret = obj.getRet();
	this->_body = obj.getBody();
	this->_port = obj.getPort();
	this->_path = obj.getPath();

	return *this;
}

const std::map<std::string, std::string>&	Request::getHeaders() const { return this->_headers; }
const std::map<std::string, std::string>&	Request::getEnv() const { return this->_env_for_cgi; }
const std::string&							Request::getMethod() const { return this->_method; }
const std::string&							Request::getVersion() const { return this->_version; }
int											Request::getRet() const { return this->_ret; }
const std::string&							Request::getBody() const { return this->_body; }
int											Request::getPort() const { return this->_port; }
const std::string&							Request::getPath() const { return this->_path; }
const std::string&							Request::getQuery() const { return this->_query; }
const std::string&							Request::getRaw() const { return this->_raw; }
const std::list<std::pair<std::string, float> >&	Request::getLang() const { return this->_lang; }

void	Request::setBody(const std::string& str)
{
	char	strip[] = {'\n', '\r'};
	this->_body.assign(str);
	for (int i = 0; i < 4; i++)
		if (this->_body.size() > 0 && this->_body[this->_body.size() - 1] == strip[i % 2])
			pop(this->_body);
		else
			break ;
}

void	Request::setRet(int ret)
{
	this->_ret = ret;
}

void	Request::setMethod(const std::string &method)
{
	this->_method = method;
}
