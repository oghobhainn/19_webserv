#include "../main.hpp"

// https://stackoverflow.com/questions/52635554/when-can-you-have-relative-urls-on-the-request-line-of-get-http-requests
std::string	get_full_urlpath(std::string url)
{
	// TODO - gotta check if url is valid (and absolute)
	// TODO - not sure it works this way
	// return (std::string("/Library/Webserver/Documents/Webserv")	+ url);

	//return (std::string("/Library/Webserver/Documents") + url);
	return (std::string(".") + url);
}

std::string	check_parsed_request(t_http_request &http_req_struct)
{
	if (http_req_struct.firstline.urlpath[0] != '/')
	{
		http_req_struct.status_code = 400;
		http_req_struct.error = true;
		return std::string("url invalide, non absolute");
	}
	else if (http_req_struct.firstline.method != "GET" //TODO - check le booleen du .conf si GET est authorised
		&& http_req_struct.firstline.method != "POST"
		&& http_req_struct.firstline.method != "DELETE") // TODO - fonction qui checke si ca fait partie d'un vecteur authorise, plus modulable
	{
			http_req_struct.status_code = 405;
			http_req_struct.error = true;
		return std::string("method not allowed");
	}
	// else if{
		// 
	// }
	http_req_struct.status_code = 200;
	return std::string("check parsing ok!");
}

void	parse_first_line(std::string firstline, t_hreq_firstline &hreq_firstline)
{
	std::vector<std::string>	split_fline;
	tokenize(firstline, ' ', split_fline);

	if (split_fline.size() == 3)
	{
		std::vector<std::string>::const_iterator i = split_fline.begin();
		hreq_firstline.method = *i++;
		hreq_firstline.urlpath = *i++;
		hreq_firstline.complete_urlpath = get_full_urlpath( hreq_firstline.urlpath );
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

void	parse_body(std::vector<std::string> body_vector, std::string &body)
{
	for(std::vector<std::string>::iterator it = body_vector.begin(); it != body_vector.end(); ++it) {
		body += *it + '\n';
	}
}

void	init_http_req(t_http_request & hrs)
{
	hrs.status_code = -1;
	hrs.error = false;
}

void	http_request_parser(char *buffer, t_http_request &http_req_struct)
{
	init_http_req(http_req_struct);
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
	parse_body(body_vector, http_req_struct.body);

	print_request(http_req_struct);

	std::cout << check_parsed_request(http_req_struct) << std::endl;
    return ;
}
