#include "../main.hpp"

void	print_response(Http_response const & resp)
{
	P("==================RESPONSE================");
	P(resp.get_http_response());
	P("------------------------------------------");
}