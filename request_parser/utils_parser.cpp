#include "../main.hpp"

void	print_request_firstline(t_hreq_firstline const & hreq_firstline_struct)
{
	P("==================REQUEST-FIRSTLINE================");
	PC("My Method: \t\t", hreq_firstline_struct.method);
	PC("My URL: \t\t", hreq_firstline_struct.urlpath);
	PC("My HTTP Version: \t", hreq_firstline_struct.httpversion);
	P("---------------------------------------------------");
}

void	print_request_header(t_hreq_header const & hreq_header_struct)
{
	P("==================REQUEST-HEADER================");
	PC("My User agent: \t\t", hreq_header_struct.user_agent);
	PC("My Host: \t\t\t", hreq_header_struct.host);
	PC("My Accept: \t\t", hreq_header_struct.accept);
	PC("My Accept encoding: \t\t", hreq_header_struct.accept_encoding);
	PC("My Accept Language: \t\t", hreq_header_struct.accept_language);
	PC("My Referer: \t\t", hreq_header_struct.referer);
	PC("My Connection: \t\t", hreq_header_struct.connection);
	PC("My Upgrade insecure request: \t", hreq_header_struct.upgrade_insecure_requests);
	PC("My If Modified Since: \t\t", hreq_header_struct.if_modified_since);
	PC("My If None Match: \t\t", hreq_header_struct.if_none_match);
	PC("My Content Type: \t\t", hreq_header_struct.content_type);
	PC("My Content Length: \t\t", hreq_header_struct.content_length);
	P("-------------------------------------------------");
}

void	print_request_body(std::string body)
{
	P("==================REQUEST-BODY================");
	P(body);
	P("----------------------------------------------");
}

void	print_request(t_http_request const & req)
{
	print_request_firstline(req.firstline);
	print_request_header(req.header);
	print_request_body(req.body);
}

std::string	remove_whitespaces(std::string str)
{
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	return str;
}

//split function
void tokenize(std::string const &str, const char delim, std::vector<std::string> &out)
{
    size_t start;
    size_t end = 0;
 
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

//True if s2 is a substring of s1
bool	is_line_in(std::string s1, std::string s2)
{
	if (s1.find(s2) != std::string::npos)
	{
		return true;
	}
	return false;
}
