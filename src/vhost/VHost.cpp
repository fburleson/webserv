#include "VHost.hpp"

VHost::VHost(void)
{
	this->_root = "res";
	this->_max_body_size = 1024;
}

VHost::~VHost(void)
{}

void	VHost::set_root(const std::string &root)
{
	this->_root = root;
}

void	VHost::set_err_page(const HTTPStatus &code, const std::string &file)
{
	this->_err_pages.insert({code, file});
}

void	VHost::set_max_body_size(const size_t &max_body_size)
{
	this->_max_body_size = max_body_size;
}

std::string	VHost::_parse_resource(const std::string &resource) const
{
	return (this->_root + resource);
}

std::vector<std::byte>	VHost::_get_err_page(const HTTPStatus &code) const
{
	if (this->_err_pages.find(code) != this->_err_pages.end())
		return (ftobyte(this->_err_pages.at(code)));
	return (generate_err_page(code));
}

t_httpresponse	VHost::process_request(const t_httprequest &request) const
{
	t_httpresponse	response;
	
	response.version = HTTP_VERSION;
	response.status = HTTP_INTERNAL_ERROR;
	response.client = request.client;
	if (request.head.find("Content-Length") != request.head.end())
	{
		if ((unsigned int)std::atoi(request.head.at("Content-Length").c_str()) > this->_max_body_size)
		{
			response.status = HTTP_TOO_LARGE;
			response.message = process_message(response.status);
			return (response);
		}
	}
	if (request.body.size() > this->_max_body_size)
	{
		response.status = HTTP_TOO_LARGE;
		response.message = process_message(response.status);
	}
	if (request.version != response.version)
	{
		response.status = HTTP_BAD_VERSION;
		response.message = process_message(response.status);
		return (response);
	}
	if (request.method == HTTP_GET)
		this->_process_get_method(request, response);
	else if (request.method == HTTP_POST)
		this->_process_post_method(request, response);
	else if (request.method == HTTP_DELETE)
		this->_process_delete_method(request, response);
	else
	{
		response.status = HTTP_BAD_METHOD;
		response.body = this->_get_err_page(HTTP_BAD_METHOD);
		response.head.insert({"Content-Type", "text/html"});
	}
	response.head.insert({"Content-Length", std::to_string(response.body.size())});
	response.message = process_message(response.status);
	return (response);
}

