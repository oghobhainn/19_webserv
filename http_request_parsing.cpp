#include "parsing.hpp"
 
std::string http_example =	"GET /software/htp/index.html HTTP 1.1\r\n"
								"Host: 192.241.213.46:6880\r\n"
                				"Upgrade-Insecure-Requests: 1\r\n"
                				"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                				"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/602.4.8 (KHTML, like Gecko) Version/10.0.3 Safari/602.4.8\r\n"
                				"Accept-Language: en-us\r\n"
                				"Accept-Encoding: gzip, deflate\r\n"
                				"Connection: keep-alive\r\n\r\n"
								"<html>SALUT TOUT LE MONDE</html>\n";

void	parse_first_line(std::string firstline, t_hreq_firstline &hreq_firstline)
{
	std::vector<std::string>	split_fline;
	tokenize(firstline, ' ', split_fline);

	if (split_fline.size() == 3)
	{
		std::vector<std::string>::const_iterator i = split_fline.begin();
		hreq_firstline.method = *i++;
		hreq_firstline.urlpath = *i++;
		hreq_firstline.httpversion = *i;
	}
}

void	parse_header(std::vector<std::string> header_vector, t_hreq_header &hreq_header_struct)
{
	for (std::vector<std::string>::const_iterator i = header_vector.begin();
	i != header_vector.end(); ++i)
	{
		std::vector<std::string> data;
		tokenize(*i, ':', data);
		if (data[0] == "User-Agent")
			hreq_header_struct.user_agent = remove_whitespaces(data[1]);
		else if (data[0] == "Host")
			hreq_header_struct.host = remove_whitespaces(data[1]);
		else if (data[0] == "Accept")
			hreq_header_struct.accept = remove_whitespaces(data[1]);
		else if (data[0] == "Accept-Language")
			hreq_header_struct.accept_language = remove_whitespaces(data[1]);
		else if (data[0] == "Accept-Encoding")
			hreq_header_struct.accept_encoding = remove_whitespaces(data[1]);
		else if (data[0] == "Referer")
			hreq_header_struct.referer = remove_whitespaces(data[1]);
		else if (data[0] == "Connection")
			hreq_header_struct.connection = remove_whitespaces(data[1]);
		else if (data[0] == "Upgrade-Insecure-Requests")
			hreq_header_struct.upgrade_insecure_requests = remove_whitespaces(data[1]);
		else if (data[0] == "If-Modified-Since")
			hreq_header_struct.if_modified_since = remove_whitespaces(data[1]);
		else if (data[0] == "If-None-Match")
			hreq_header_struct.if_none_match = remove_whitespaces(data[1]);
		else if (data[0] == "Content-Type")
			hreq_header_struct.content_type = remove_whitespaces(data[1]);
		else if (data[0] == "Content-Length")
			hreq_header_struct.content_length = remove_whitespaces(data[1]);
		else if (data[0] == "Cache-Control")
			hreq_header_struct.cache_control = remove_whitespaces(data[1]);
	}
}

//Parsing:
//prend une requete de type 
// std::string http_example =	"GET /software/htp/index.html HTTP 1.1\r\n"
// 								"Host: 192.241.213.46:6880\r\n"
//                 				"Upgrade-Insecure-Requests: 1\r\n"
//                 				"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
//                 				"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/602.4.8 (KHTML, like Gecko) Version/10.0.3 Safari/602.4.8\r\n"
//                 				"Accept-Language: en-us\r\n"
//                 				"Accept-Encoding: gzip, deflate\r\n"
//                 				"Connection: keep-alive\r\n\r\n"
// 								"<html>SALUT TOUT LE MONDE</html>\n";
// cree une structure de vecteurs.
// firstline, contenant METHOD - URL - HTTP VERSION
// header, contenant la valeur de chaque data
// body, contenant le body



void	http_request_parser(char *buffer, t_http_request &http_req_struct)
{
	std::string					firstline_str;
	std::vector<std::string>	request_vector;
	std::vector<std::string>	header_vector;
	std::vector<std::string>	body_vector;
	bool						header_done_bool = false;
	bool						firstline_done_bool = false;

	tokenize(buffer, '\n', request_vector);

	for ( std::vector<std::string>::const_iterator i = request_vector.begin(); i != request_vector.end(); ++i)
	{
		if (firstline_done_bool == false)
		{
			firstline_str = *i;
			firstline_done_bool = true;
		}
		else if (*i != "\r" && !header_done_bool )
			header_vector.push_back(*i);
		else if (*i == "\r" && !header_done_bool)
			header_done_bool = true;
		else
			body_vector.push_back(*i);
	}

	parse_first_line(firstline_str, http_req_struct.firstline);
	parse_header(header_vector, http_req_struct.header);

	//print_request_firstline(http_req_struct.firstline);
	//print_request_header(http_req_struct.header);

    return ;
}

// int main()
// {
//     std::string s = http_example;
//     const char delim = '\n';
 
// 	std::vector<std::string>	request_vector;
// 	std::string 				firstline_str;
// 	std::vector<std::string>	header_vector;
// 	std::vector<std::string>	body_vector;

// 	t_hreq_firstline			hreq_firstline_struct;
// 	t_hreq_header				hreq_header_struct;

// 	bool						header_done_bool = false;
// 	bool						firstline_done_bool = false;

//     tokenize(s, delim, request_vector);
 
// 	for ( std::vector<std::string>::const_iterator i = request_vector.begin(); i != request_vector.end(); ++i)
// 	{
// 		if (firstline_done_bool == false)
// 		{
// 			firstline_str = *i;
// 			firstline_done_bool = true;
// 		}
// 		else if (*i != "\r" && !header_done_bool )
// 			header_vector.push_back(*i);
// 		else if (*i == "\r" && !header_done_bool)
// 			header_done_bool = true;
// 		else
// 			body_vector.push_back(*i);
// 	}

// 	parse_first_line(firstline_str, hreq_firstline_struct);
// 	parse_header(header_vector, hreq_header_struct);

// 	print_request_firstline(hreq_firstline_struct);
// 	print_request_header(hreq_header_struct);

//     return 0;
// }

