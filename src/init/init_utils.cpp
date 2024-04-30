/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:10:46 by bjacobs           #+#    #+#             */
/*   Updated: 2024/04/29 08:36:03 by bjacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initialization.hpp"
#include <string>

std::string::iterator	getFirstWord(std::string::iterator it) {
	while (std::isspace(*it) && *it) 
		++it;
	return (it);
}

std::string::iterator	skipWord(const std::string::iterator &it) {
	std::string::iterator	wordEnd;

	wordEnd = it;
	while (!std::isspace(*wordEnd) && *wordEnd) 
		++wordEnd;
	return (wordEnd);
}

std::string::iterator&	getNextWord(std::string::iterator &it) {
	it = skipWord(it);
	it = getFirstWord(it);
	return (it);
}

std::string	IPtoString(const uint32_t &ip) {
	std::string	IPstring;
	int8_t i;

	for (i = 3; i; --i)
		IPstring += std::to_string(((ip >> (i * 8)) & 0xFF)) + '.';
	IPstring += std::to_string(((ip >> (i * 8)) & 0xFF));
	return (IPstring);
}

void	setDefault(t_conf &conf) {
	conf.ip = DEFAULT_IP;
	conf.max_body_size = DEFAULT_BODY;
	conf.port = DEFAULT_PORT;
	conf.default_route.autoindex = DEFAULT_AUTOINDEX;
}
