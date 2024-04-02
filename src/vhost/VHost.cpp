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

t_httpresponse	VHost::process_request(const t_httprequest &request) const
{
	t_httpresponse	response;

	response.status = HTTP_OK; 
	response.message = "OK";
	response.version = "HTTP/1.1";
	response.body = ftobyte(this->_root + request.resource);
	response.head.insert({"Content-Length", std::to_string(response.body.size())});
	response.head.insert({"Content-Type", "text/html"});
	response.client = request.client;
	return (response);
}

