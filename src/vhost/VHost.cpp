#include "VHost.hpp"

VHost::VHost(void)
{
	this->_default_route.index = "/index.html";
	this->_default_route.autoindex = false;
	this->_max_body_size = 1024;
}

VHost::~VHost(void)
{}

void	VHost::init(const t_conf &conf)
{
	this->_default_route = conf.default_route;
	this->_routes = conf.routes;
	this->_err_pages = conf.err_pages;
	this->_max_body_size = conf.max_body_size;
}

void	VHost::set_root(const std::string &root)
{
	this->_default_route.root = root;
}

void	VHost::set_index(const std::string &index)
{
	this->_default_route.index = index;
}

void	VHost::set_autoindex(const bool &autoindex)
{
	this->_default_route.autoindex = autoindex;
}

void	VHost::set_redirect(const std::string &url)
{
	this->_default_route.redirect = url;
}

void	VHost::set_err_page(const HTTPStatus &code, const std::string &file)
{
	this->_err_pages.insert({code, file});
}

void	VHost::set_max_body_size(const size_t &max_body_size)
{
	this->_max_body_size = max_body_size;
}

void	VHost::allow_method(const HTTPMethod &method)
{
	this->_default_route.allowed_methods.insert(method);
}

void	VHost::add_route(const t_route &route)
{
	this->_routes.push_back(route);
}

bool	VHost::_is_too_large(const t_httprequest &request) const
{
	if (request.head.find("Content-Length") != request.head.end())
	{
		if ((unsigned int)std::atoi(request.head.at("Content-Length").c_str()) > this->_max_body_size)
			return (true);
	}
	if (request.body.size() > this->_max_body_size)
		return (true);
	return (false);
}

bool	VHost::_is_incomplete_dir(const t_httprequest &request, const t_route &route) const
{
	fs::path	path = fs::path(parse_resource(request.url, route));

	if (fs::is_directory(path))
	{
		if (request.url.back() != '/')
			return (true);
	}
	return (false);
}

std::vector<std::byte>	VHost::_get_err_page(const HTTPStatus &code) const
{
	if (this->_err_pages.find(code) != this->_err_pages.end())
		return (ftobyte(this->_err_pages.at(code)));
	return (generate_err_page(code));
}

t_route	VHost::_get_route(const t_httprequest &request) const
{
	t_route	selected_route = this->_default_route;

	for (const t_route &route : this->_routes)
	{
		if (request.url.starts_with(route.alias))
			selected_route = route;
	}
	return (selected_route);
}

t_httpresponse	VHost::_process_error(const HTTPStatus &code) const
{
	t_httpresponse	response;

	response.status = code;
	response.body = this->_get_err_page(code);
	response.head.insert({"Content-Type", "text/html"});
	return (response);
}

static std::string	method_to_str(const HTTPMethod &method)
{
	if (method == HTTP_GET)
		return ("GET");
	if (method == HTTP_POST)
		return ("POST");
	if (method == HTTP_DELETE)
		return ("DELETE");
	return ("UNKNOWN METHOD");
}

static std::string	format_allowed_methods(const t_route &route)
{
	std::stringstream	buffer;
	
	for (const HTTPMethod &method : route.allowed_methods)
	{
		if (!buffer.str().empty())
			buffer << ", ";
		buffer << method_to_str(method);
	}
	return (buffer.str());
}

t_httpresponse	VHost::_process_bad_method(const HTTPStatus &code, const t_route &route) const
{
	t_httpresponse	response;
	std::string	allowed = route.alias;

	response = this->_process_error(code);
	response.head.insert({"Allow", format_allowed_methods(route)});
	return (response);
}

t_httpresponse	VHost::process_request(const t_httprequest &request) const
{
	t_httpresponse	response;
	t_route		route = this->_get_route(request);

	if (request.version != HTTP_VERSION)
		response = this->_process_error(HTTP_BAD_VERSION);
	else if (!is_method_allowed(request.method, route))
		response = this->_process_bad_method(HTTP_BAD_METHOD, route);
	else if (this->_is_too_large(request))
		response = this->_process_error(HTTP_TOO_LARGE);
	else if (!route.redirect.empty())
		response = process_redirect(route.redirect);
	else if (this->_is_incomplete_dir(request, route))
		response = process_redirect(request.url + '/');
	else if (request.method == HTTP_GET)
		response = this->_process_get_method(request, route);
	else if (request.method == HTTP_POST)
		response = this->_process_post_method(request, route);
	else if (request.method == HTTP_DELETE)
		response = this->_process_delete_method(request, route);
	response.version = HTTP_VERSION;
	response.client = request.client;
	response.message = process_message(response.status);
	response.head.insert({"Server", SERVER_NAME});
	response.head.insert({"Content-Length", std::to_string(response.body.size())});
	return (response);
}

