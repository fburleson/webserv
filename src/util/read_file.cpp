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
	char		buffer[BUFF_READ_SIZE];
	int		status;

	buffer[0] = '\0';
	status = read(fd, buffer, BUFF_READ_SIZE);
	if (status <= 0)
		buffer[0] = '\0';
	buffer[status] = '\0';
	return (buffer);
}

