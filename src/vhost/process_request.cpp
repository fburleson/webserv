#include "VHost.hpp"

t_httpresponse	VHost::_process_get_method(const t_httprequest &request, const t_route &route) const
{
	t_httpresponse	response;
	std::string	resource = this->_parse_resource(request.resource, route);
	fs::path	path = fs::path(resource);
	
	if (resource.back() == '/')
	{
		if (route.autoindex && fs::is_directory(path))
		{
			response.status = HTTP_OK;
			response.body = generate_dir_list(request.resource, route.root);
			response.head.insert({"Content-Type", "text/html"});
			return (response);
		}
		return (process_redirect(route.index));
	}
	if (!fs::exists(path))
	{
		response.status = HTTP_NOT_FOUND;
		response.body = this->_get_err_page(HTTP_NOT_FOUND);
		return (response);
	}
	response.status = HTTP_OK;
	response.body = ftobyte(resource);
	response.head.insert({"Content-Location", resource});
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
	resource = this->_parse_resource(request.resource, route);
	std::ofstream	new_file(resource);

	for (const std::byte &byte : request.body)
		new_file << (char)byte;
	response.status = HTTP_NO_CONTENT;
	response.head.insert({"Content-Location", resource});
	return (response);
}

t_httpresponse	VHost::_process_delete_method(const t_httprequest &request, const t_route &route) const
{
	t_httpresponse	response;
	std::string	resource = this->_parse_resource(request.resource, route);
	int		status = std::remove(resource.c_str());	
	
	if (status != 0)
		response.status = HTTP_NOT_FOUND;
	if (status == 0)
	{
		response.status = HTTP_NO_CONTENT;
		response.head.insert({"Content-Location", resource});
	}
	return (response);
}

