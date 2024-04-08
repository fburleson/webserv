#include "webserv.hpp"

std::string	process_message(const HTTPStatus &code)
{
	if (code == HTTP_OK)
		return ("OK");
	if (code == HTTP_NOT_FOUND)
		return ("Not Found");
	if (code == HTTP_BAD_VERSION)
		return ("HTTP Version Not Supported");
	if (code == HTTP_BAD_METHOD)
		return ("Method Not Allowed");
	if (code == HTTP_CONTINUE)
		return ("Continue");
	if (code == HTTP_CREATED)
		return ("Created");
	if (code == HTTP_NO_CONTENT)
		return ("No Content");
	if (code == HTTP_TOO_LARGE)
		return ("Payload Too Large");
	return ("Internal Server Error");
}

std::vector<std::byte>	generate_err_page(const HTTPStatus &code)
{
	std::stringstream	buffer;
	std::string		message = process_message(code);

	buffer << "<!DOCTYPE html>";
	buffer << "<html>";
	buffer << "<head><title>" << code << " " << message << "</title></head>";
	buffer << "<body>";
	buffer << "<div style=\"text-align:center\">";
	buffer << "<h1>" << code << " " << message << "</h1>";
	buffer << "<hr>";
	buffer << "<p>webserv</p>";
	buffer << "</div>";
	buffer << "</body>";
	buffer << "</html>";
	return (stobyte(buffer.str()));
}

bool	is_method_allowed(const HTTPMethod &method, const t_route &route)
{
	return (route.allowed_methods.find(method) != route.allowed_methods.end());
}

