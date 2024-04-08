#include "webserv.hpp"

std::vector<std::byte>	stobyte(const std::string &body)
{
	std::vector<std::byte>	bytes;
	
	for (const char &c : body)
		bytes.push_back((std::byte)c);	
	return (bytes);
}

std::vector<std::byte>	ftobyte(const std::string &file)
{
	std::vector<std::byte>	data;
	std::ifstream		file_stream(file);
	std::stringstream	content;
	std::string		buffer;

	if (!file_stream.is_open())
		return (data);
	while (std::getline(file_stream, buffer))
		content << buffer << '\n';
	data = stobyte(content.str());
	return (data);
}

static std::string	format_response(const t_httpresponse &response)
{
	std::stringstream	data;
	
	data << response.version << ' ';
	data << response.status << ' ';
	data << response.message << "\r\n";
	for (const auto &[key, value] : response.head)
		data << key << ':' << value << "\r\n";
	data << "\r\n";
	for (const std::byte &byte : response.body)
      		data << (char)byte;
	return (data.str());
}

void	send_response(t_httpresponse &response)
{
	std::string	formatted;

	formatted = format_response(response);
	if (send(response.client.fd, formatted.c_str(), formatted.size(), 0) == -1)
		std::cerr << " send(): " << strerror(errno) << std::endl;
}

