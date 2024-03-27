#include "webserv.hpp"

int	error(const std::string &msg, int err_code)
{
	std::cerr << msg << " (code: " << err_code << ")" << std::endl;
	return (err_code);
}

