#include "VHost.hpp"

static std::vector<std::byte>	generate_default_html(const std::string &title, const std::string &message)
{
	std::stringstream	buffer;

	buffer << "<!DOCTYPE html>";
	buffer << "<html>";
	buffer << "<head><title>" << title << "</title></head>";
	buffer << "<body>";
	buffer << "<h1>" << title << "</h1>";
	buffer << "<p>" << message << "</p>";
	buffer << "</body>";
	buffer << "</html>";
	return (stobyte(buffer.str()));
}

std::vector<std::byte>	VHost::_process_body(const t_httprequest &request, const t_httpresponse &response) const
{
	if (response.status == HTTP_OK)
		return (ftobyte(this->_parse_resource(request.resource)));
	if (this->_err_pages.find(response.status) != this->_err_pages.end())
		return (ftobyte(this->_err_pages.at(response.status)));
	if (response.status == HTTP_NOT_FOUND)
		return (generate_default_html("Error 404", "Page Not Found"));
	if (response.status == HTTP_BAD_VERSION)
		return (generate_default_html("Error 505", "HTTP Version Not Supported"));
	return (generate_default_html("Error 500", "Internal Server Error"));
}

