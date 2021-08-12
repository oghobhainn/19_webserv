#include "../main.hpp"
/*
void	print_request_firstline(t_hreq_firstline const & hreq_firstline_struct)
{
	P("My Method: ", hreq_firstline_struct.method);
	P("My URL: ", hreq_firstline_struct.urlpath);
	P("My HTTP Version: ", hreq_firstline_struct.httpversion);
}

void	print_request_header(t_hreq_header const & hreq_header_struct)
{
	P("My User agent: ", hreq_header_struct.user_agent);
	P("My Host: ", hreq_header_struct.host);
	P("My Accept: ", hreq_header_struct.accept);
	P("My Accept encoding: ", hreq_header_struct.accept_encoding);
	P("My Accept Language: ", hreq_header_struct.accept_language);
	P("My Referer: ", hreq_header_struct.referer);
	P("My Connection: ", hreq_header_struct.connection);
	P("My Upgrade insecure request: ", hreq_header_struct.upgrade_insecure_requests);
	P("My If Modified Since: ", hreq_header_struct.if_modified_since);
	P("My If None Match: ", hreq_header_struct.if_none_match);
	P("My Content Type: ", hreq_header_struct.content_type);
	P("My Content Length: ", hreq_header_struct.content_length);
}
*/
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
