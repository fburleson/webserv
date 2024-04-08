#include "webserv.hpp"

bool	path_exists(const std::string &path)
{
	struct stat	buffer;

	return (stat(path.c_str(), &buffer) == 0);
}

bool	is_dir(const std::string &path)
{
	return (path.back() == '/');
}
