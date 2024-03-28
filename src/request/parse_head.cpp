#include "webserv.hpp"

static std::string	get_line_stripped(const std::string &line)
{
	std::stringstream	buffer;

	for (const char &c : line)
	{
		if (c == '\n')
			break ;
		if (std::isspace(c))
			continue ;
		buffer << c;
	}
	return (buffer.str());
}

static std::string	get_key(const std::string &key_val)
{
	std::stringstream	key;
	
	for (const char &c : key_val)
	{
		if (c == ':')
			break ;
		key << c;
	}
	return (key.str());
}

static std::string	get_value(const std::string &key_val)
{
	unsigned int	start = 0;

	for (const char &c : key_val)
	{
		start++;
		if (c == ':')
			break ;
	}
	return (key_val.substr(start));
}

std::map<std::string, std::string>	parse_head(const std::string &head)
{
	std::map<std::string, std::string>	parsed;
	unsigned int				cidx = 0;
	std::string				current_line;
	
	current_line = get_line_stripped(head);
	while (!current_line.empty())
	{
		parsed.insert({get_key(current_line), get_value(current_line)});
		for (const char &c : head.substr(cidx))
		{
			cidx++;
			if (c == '\n')
				break ;
		}
		current_line = get_line_stripped(head.substr(cidx));
	}
	return (parsed);
}

