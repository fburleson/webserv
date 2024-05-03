/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_functions.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 22:31:35 by bjacobs           #+#    #+#             */
/*   Updated: 2024/05/01 00:11:34 by bjacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initialization.hpp"
#include <stdexcept>
#include <cstring>

uint32_t	getIP(std::string::iterator word) {
	std::string::iterator	start = word;
	uint32_t				ip = 0;
	uint16_t				byte = 0;
	int8_t					shifts = 24;

	while (*word && !std::isspace(*word)) {
		if (*word == '.') {
			if (shifts < 0)
				throw ConfigException(ERR_IP);
			ip |= (byte << shifts);
			byte = 0;
			shifts -= 8;
		}
		else if (std::isdigit(*word)) {
			byte = byte * 10 + *word - '0';
			if (byte > 255) {
				throw ConfigException(std::string(ERR_IP)
						+ " \"" +  &(*start) + "\"");
			}
		}
		else {
			throw ConfigException(std::string(ERR_IP)
					+ " \"" +  &(*start) + "\"");
		}
		++word;
	}
	parseLeftOverString(word, "ip");
	ip |= byte;
	return (ip);
}

uint8_t	getKeywordIndex(const std::string::iterator &word) {
	std::string		keyWords[9] = {"listen", "server_names", "root", "autoindex", "ip",
									"allowed",  "max_body_size", "err_page", "index"};
	unsigned char	wordEnd;
	size_t			keyWordLen;
	uint8_t			i;

	for (i = 0; i < 9; ++i) {
		keyWordLen = keyWords[i].size();
		if (!keyWords[i].compare(0, keyWordLen,  &(*word), keyWordLen)) {
			wordEnd = word[keyWordLen];
			if (wordEnd != ':' && !std::isspace(wordEnd))
				continue;
			break;
		}
	}
	if (i > 8) {
		throw ConfigException(std::string(ERR_UNKNOWN_KEY) + "\"" 
				+ std::string(word, skipWord(word)) + "\"");
	}
	return (i);
}

enum HTTPMethod	getMethodEnum(const std::string &method) {
	std::string		methodList[3] = {"get", "post", "delete"};
	unsigned char	methodEnd;
	uint8_t			i;

	for (i = 0; i < 3; ++i) {
		if (!method.compare(0, methodList[i].size(), methodList[i])) {
			methodEnd = method[methodList[i].length()];
			if (methodEnd && !std::isspace(methodEnd))
				continue;
			break;
		}
	}
	return (static_cast<HTTPMethod>(i));
}

int	getNumberValue(std::string::iterator &start) {
	std::string::iterator	number;
	int						value;

	number = start;
	while (*start && !std::isspace(*start)) {
		if (!std::isdigit(*start))
			throw ConfigException(std::string(ERR_MSG_START) + "\""
				+ std::string(number, skipWord(start)) + "\" is not a number");
		++start;
	}
	try {
		value = std::stoi(&(*number));
	}
	catch (std::out_of_range& e) {
		throw ConfigException(std::string(ERR_MSG_START) + "\""
			+ std::string(number, skipWord(start)) + "\" is too large a number");
	}
	return (value);
}

std::string	getWordValue(std::string::iterator &end) {
	std::string::iterator	start;

	for (start = end; !std::isspace(*end) && *end; ++end) {
		if (std::strchr("{", *end) && (end == start || *(end-1) != '\\'))
			throw ConfigException(std::string(ERR_OPEN_BRACKET) + " \"" + &(*start) + "\"");
	}
	return (std::string(start, end));
}

bool	getAutoIndex(std::string::iterator& word) {
	std::string	value;

	value = getWordValue(word);
	if (value == "on")
		return (true);
	if (value == "off")
		return (false);
	throw ConfigException(std::string(ERR_MSG_START)
			+ "autoindex value \"" + value + "\" not recognized");
}
