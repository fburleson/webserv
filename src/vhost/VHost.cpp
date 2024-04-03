#include "VHost.hpp"

VHost::VHost(void)
{
	this->_root = "res";
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

std::string	VHost::_parse_resource(const std::string &resource) const
{
	return (this->_root + resource);
}

HTTPStatus	VHost::_process_code(const t_httprequest &request) const
{
	HTTPStatus	code = HTTP_INTERNAL_ERROR;

	if (request.version != "HTTP/1.1")
		return (HTTP_BAD_VERSION);
	if (request.method == HTTP_GET)
	{
		if (file_exists(this->_parse_resource(request.resource)))
			code = HTTP_OK;
		else
			code = HTTP_NOT_FOUND;
	}
	return (code);
}

static std::string	process_message(const HTTPStatus &code)
{
	if (code == HTTP_OK)
		return ("OK");
	if (code == HTTP_NOT_FOUND)
		return ("Not Found");
	if (code == HTTP_BAD_VERSION)
		return ("HTTP Version Not Supported");
	return ("Internal Server Error");
}

t_httpresponse	VHost::process_request(const t_httprequest &request) const
{
	t_httpresponse	response;

	response.version = "HTTP/1.1";
	response.status = this->_process_code(request); 
	response.body = this->_process_body(request, response);
	response.head.insert({"Content-Length", std::to_string(response.body.size())});
	response.head.insert({"Content-Type", "text/html"});
	response.message = process_message(response.status);
	response.client = request.client;
	return (response);
}

