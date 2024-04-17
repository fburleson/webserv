#include "webserv.hpp"

std::string	read_file(const std::string &path)
{
	std::ifstream		file(path);
	std::stringstream	buffer;
	std::string		line;

	while (getline(file, line))
		buffer << line << '\n';
	return (buffer.str());
}

std::string	read_file(const int &fd)
{
	std::stringstream	content;
	char			buffer[BUFF_READ_SIZE];
	int			status;

	buffer[0] = '\0';
	while (status > 0)
	{
		status = read(fd, buffer, BUFF_READ_SIZE - 1);
		if (status == -1)
		{
			content.clear();
			return (content.str());
		}
		buffer[status] = '\0';
		content << buffer;
	}
	return (content.str());
}

