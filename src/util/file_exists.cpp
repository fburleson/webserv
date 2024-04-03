#include "webserv.hpp"

bool	file_exists(const std::string &path)
{
	struct stat	buffer;

	return (stat(path.c_str(), &buffer) == 0);
}

