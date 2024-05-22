#include "VHost.hpp"
#include "CgiHandler.hpp"

t_httpresponse	VHost::_process_cgi(const t_httprequest &request, const t_route &route) const
{
	t_httpresponse	response;
	std::string		output;
	size_t			i;
	std::string		resource = parse_resource(request.url, route);

	if (access(resource.c_str(), F_OK))
	{
		response = this->_process_error(HTTP_NOT_FOUND);
		return (response);
	}
	if (access(resource.c_str(), X_OK))
	{
		response = this->_process_error(HTTP_FORBIDDEN);
		return (response);
	}
	CgiHandler	handler(request, route);
	if (handler.startExecution() == -1)
	{
		response = this->_process_error(HTTP_INTERNAL_ERROR);
		return (response);
	}
	if (!handler.startExecution())
	{
		response = this->_process_error(HTTP_TIMEOUT);
		return (response);
	}
	output = handler.getOutput();
	response.status = HTTP_OK;
	response.head = parse_head(output);
	for (i = 0; output[i]; ++i) {
		if (output[i] == '\n' && std::isspace(output[i + 1]))
			break;
	}
	while (std::isspace(output[i]) && output[i])
		++i;
	response.body = parse_body(output.substr(i));
	return (response);
}

t_httpresponse	VHost::_process_get_method(const t_httprequest &request, const t_route &route) const
{
	t_httpresponse	response;
	std::string	resource = parse_resource(request.url, route);
	fs::path	path = fs::path(resource);

	if (resource.back() == '/')
	{
		if (route.autoindex && fs::is_directory(path))
		{
			response.status = HTTP_OK;
			response.body = generate_dir_list(request, route);
			response.head.insert({"Content-Type", "text/html"});
			return (response);
		}
		if (route.index.empty())
			return (this->_process_error(HTTP_NOT_FOUND));
		return (process_redirect(route.alias + route.index));
	}
	if (!fs::exists(path))
	{
		response = this->_process_error(HTTP_NOT_FOUND);
		return (response);
	}
	response.status = HTTP_OK;
	response.body = ftobyte(resource);
	response.head.insert({"Content-Location", request.url});
	response.head.insert({"Content-Type", get_content_type(resource)});
	return (response);
}

t_httpresponse	VHost::_process_post_method(const t_httprequest &request, const t_route &route) const
{
	t_httpresponse	response;
	std::string	resource;

	if (request.head.find("Expect") != request.head.end())
	{
		if (request.head.at("Expect") == "100-continue")
		{
			response.status = HTTP_CONTINUE;
			return (response);
		}
	}
	resource = parse_resource(request.url, route);
	if (resource == route.root || resource == (route.root + '/'))
	{
		std::cout << "Resource: " << resource << std::endl;
		response = this->_process_error(HTTP_BAD_REQUEST);
		return (response);
	}
	std::ofstream	new_file(resource);

	for (const std::byte &byte : request.body)
		new_file << (char)byte;
	response.status = HTTP_NO_CONTENT;
	return (response);
}

t_httpresponse	VHost::_process_delete_method(const t_httprequest &request, const t_route &route) const
{
	t_httpresponse	response;
	std::string	resource = parse_resource(request.url, route);
	int		status;	

	if (resource == route.root || resource == (route.root + '/'))
	{
		response = this->_process_error(HTTP_FORBIDDEN);
		return (response);
	}
	status = std::remove(resource.c_str());
	if (status != 0)
		response = this->_process_error(HTTP_INTERNAL_ERROR);
	if (status == 0)
		response.status = HTTP_NO_CONTENT;
	return (response);
}

