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
	if (code == HTTP_PERM_MOVE)
		return ("Moved Permanently");
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

std::vector<std::byte>	generate_dir_list(const std::string &resource, const std::string &root)
{
	std::stringstream	buffer;
	std::string		dir = root + resource;
	std::string		current_path;
	
	buffer << "<!DOCTYPE html>";
	buffer << "<html>";
	buffer << "<head><title>Index of " << resource << "</title></head>";
	buffer << "<body>";
	buffer << "<h1>Index of " << resource << "</h1>";
	buffer << "<hr>";
	current_path = fs::path(resource).parent_path().parent_path();
	if (current_path.back() != '/')
		current_path += '/';
	buffer << "<a href=" << current_path << ">../" << "</a><br>";
	for (const auto &entry : fs::directory_iterator(dir))
	{
		if (entry.is_directory())
		{
			current_path = entry.path().filename();
			current_path += '/';
		}
		else
			current_path = entry.path().filename();
		buffer << "<a href=" << current_path << ">" << current_path << "</a><br>";
	}
	buffer << "<hr>";
	buffer << "</body>";
	buffer << "</html>";
	return (stobyte(buffer.str()));
}

t_httpresponse	process_redirect(const std::string &url)
{
	t_httpresponse	response;

	response.status = HTTP_PERM_MOVE;
	response.head.insert({"Location", url});
	return (response);
}

bool	is_method_allowed(const HTTPMethod &method, const t_route &route)
{
	return (route.allowed_methods.find(method) != route.allowed_methods.end());
}

