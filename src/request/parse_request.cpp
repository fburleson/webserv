#include "webserv.hpp"

static unsigned int	skip_next_info(const std::string &request, const unsigned int &req_cidx)
{
	unsigned int	cidx = req_cidx;

	for (const char &c : request.substr(cidx))
	{
		if (std::isspace(c))
			break ;
		cidx++;
	}
	for (const char &c : request.substr(cidx))
	{
		if (!std::isspace(c))
			break ;
		cidx++;
	}
	return (cidx);
}

static std::string	get_next_info(const std::string &start)
{
	std::stringstream	info;
	
	for (const char &c : start)
	{
		if (std::isspace(c))
			break ;
		info << c;
	}
	return (info.str());
}

static HTTPMethod	get_method(const std::string &request)
{
	std::string	method;

	method = get_next_info(request);
	if (method == "GET")
		return (HTTP_GET);
	else if (method == "POST")
		return (HTTP_POST);
	else if (method == "DELETE")
		return (HTTP_DELETE);
	else
		return (HTTP_UNKNOWN);
}

static std::vector<std::byte>	parse_body(const std::string &body)
{
	std::vector<std::byte>	parsed;
	
	for (const char &c : body)
		parsed.push_back((std::byte)c);
	return (parsed);
}

t_httprequest	parse_request(const std::string &request)
{
	t_httprequest	parsed;
	unsigned int	cidx = 0;
	
	parsed.method = get_method(request);
	cidx = skip_next_info(request, cidx);
	parsed.resource = get_next_info(request.substr(cidx));
	cidx = skip_next_info(request, cidx);
	parsed.version = get_next_info(request.substr(cidx));
	cidx = skip_next_info(request, cidx);
	parsed.head = parse_head(request.substr(cidx));
	for (const char &c : request.substr(cidx))
	{
		if (c == '\n' && std::isspace(request[cidx + 1]))
			break ;
		cidx++;
	}
	while (std::isspace(request[cidx]))
		cidx++;
	parsed.body = parse_body(request.substr(cidx));
	return (parsed);
}

