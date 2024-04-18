#include "webserv.hpp"

std::string	remove_query_string(const std::string &url)
{
	return (url.substr(0, url.find('?')));
}

std::string	parse_resource(const std::string &url, const t_route &route)
{
	std::string	parsed;
	std::string	url_no_query;
	
	url_no_query = url;
	if (url.contains('?'))
		url_no_query = remove_query_string(url);
	if (url_no_query.length() > route.alias.length())
		parsed = route.root + url_no_query.substr(route.alias.length());
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
	if (code == HTTP_BAD_REQUEST)
		return ("Bad Request");
	if (code == HTTP_FORBIDDEN)
		return ("Forbidden");
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
	struct stat		file_stat;

	buffer << "<!DOCTYPE html>";
	buffer << "<html>";
	buffer << "<head><title>Index of " << request.url << "</title></head>";
	buffer << "<body>";
	buffer << "<h1>Index of ";
	if (request.url.contains('?'))
		buffer << remove_query_string(request.url);
	else
		buffer << request.url;
	buffer << "</h1>";
	buffer << "<hr>";
	buffer << "<table style=\"width:100%\">";
	buffer << "<tr style=\"padding:0px\">";
	buffer << "<th style=\"text-align:left\">name</th>";
	buffer << "<th style=\"text-align:left\">size</th>";
	buffer << "<th style=\"text-align:left\">last modified</th>";
	buffer << "</tr>";
	if (route.root + '/' != dir)
	{
		buffer << "<tr>";
		buffer << "<td><a href=" << fs::path(request.url).parent_path().parent_path() << ">../" << "</a></td>";
		buffer << "<td></td>";
		buffer << "<td>";
		if (stat(fs::path(request.url).parent_path().parent_path().c_str(), &file_stat) == 0)
			buffer << std::ctime(&file_stat.st_mtime) << "</td>";
		else
			buffer << "unknown</td>";
		buffer << "</tr>";
	}
	for (const auto &entry : fs::directory_iterator(dir))
	{
		current_path = entry.path().filename();
		buffer << "<tr>";
		if (!entry.is_directory())
		{
			buffer << "<td><a href=" << current_path << ">" << current_path << "</a></td>";
			buffer << "<td>" << entry.file_size() << "</td>";
		}
		else
		{
			current_path += '/';
			buffer << "<td><a href=" << current_path << ">" << current_path << "</a></td>";
			buffer << "<td></td>";
		}
		buffer << "<td>";
		if (stat(entry.path().c_str(), &file_stat) == 0)
			buffer << std::ctime(&file_stat.st_mtime) << "</td>";
		else
			buffer << "unknown</td>";
		buffer << "</tr>";
	}
	buffer << "</table>";
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
	if (extension == ".jpeg" || extension == ".jpg")
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

