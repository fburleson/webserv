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
	std::string	content;
	char		buffer[BUFF_READ_SIZE];

	read(fd, buffer, BUFF_READ_SIZE);
	content = buffer;
	return (content);
}

