#include "webserv.hpp"

std::string	parse_resource(const std::string &url, const t_route &route)
{
	std::string	parsed;

	if (url.length() > route.alias.length())
		parsed = route.root + url.substr(route.alias.length());
	else
		parsed = route.root;
	return (parsed);
}

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
	buffer << "<center><h1>" << code << " " << message << "</h1></center>";
	buffer << "<hr>";
	buffer << "<center><p>" << SERVER_NAME << "</p></center>";
	buffer << "</body>";
	buffer << "</html>";
	return (stobyte(buffer.str()));
}

std::vector<std::byte>	generate_dir_list(const t_httprequest &request, const t_route &route)
{
	std::stringstream	buffer;
	std::string		dir = parse_resource(request.url, route);
	std::string		current_path;
	
	buffer << "<!DOCTYPE html>";
	buffer << "<html>";
	buffer << "<head><title>Index of " << request.url << "</title></head>";
	buffer << "<body>";
	buffer << "<h1>Index of " << request.url << "</h1>";
	buffer << "<hr>";
	if (route.root + '/' != dir)
		buffer << "<a href=" << fs::path(request.url).parent_path().parent_path() << ">../" << "</a><br>";
	for (const auto &entry : fs::directory_iterator(dir))
	{
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

std::string	get_content_type(const std::string &resource)
{
	std::string	extension = fs::path(resource).extension();

	if (extension == ".pdf")
		return ("application/pdf");
	if (extension == ".json")
		return ("application/json");
	if (extension == ".gif")
		return ("image/gif");
	if (extension == ".jpeg")
		return ("image/jpeg");
	if (extension == ".png")
		return ("image/png");
	if (extension == ".css")
		return ("text/css");
	if (extension == ".html")
		return ("text/html");
	if (extension == ".js")
		return ("text/javascript");
	if (extension == ".txt")
		return ("text/plain");
	if (extension == ".mp4")
		return ("video/mp4");
	if (extension == ".webm")
		return ("video/webm");
	return ("application/octet-stream");
}

bool	is_method_allowed(const HTTPMethod &method, const t_route &route)
{
	return (route.allowed_methods.find(method) != route.allowed_methods.end());
}

