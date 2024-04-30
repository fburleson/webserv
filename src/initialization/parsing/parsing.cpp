/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 22:28:48 by bjacobs           #+#    #+#             */
/*   Updated: 2024/04/30 05:21:12 by bjacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initialization.hpp"
#include <string>
#include <sys/stat.h>

bool	parsePath(const std::string &path, struct stat *buff) {
	if (stat(path.c_str(), buff) == -1)
		return (false);
	return (true);
}

void	parseLeftOverString(std::string::iterator &start, const std::string &keyWord) {
	while (*start && *start != '}') {
		if (!std::isspace(*start))
			throw ConfigException(std::string(ERR_NUM_ARGS) + keyWord);
		++start;
	}
}

void	parseSeparator(std::string::iterator &word) {
	while (!std::isspace(*(word+1)) && *word != ':' && *word)
		++word;
	if (*word != ':' && *getNextWord(word) != ':')
		throw ConfigException(std::string(ERR_SEPARATOR));
	if (std::isspace(*(word+1)) && !*getNextWord(word) && *word != '}')
		throw ConfigException(std::string(ERR_NO_VALUE));
	if (*word == ':')
		++word;
}

uint8_t	parseConfigs(std::vector<t_conf> &serverConfigs) {
	struct stat	buff;
	size_t		size;

	for (t_conf conf : serverConfigs) {
		for (const std::pair<const HTTPStatus, std::string> &kv : conf.err_pages) {
			if (!parsePath(kv.second, &buff)) {
				printError(std::string(ERR_MSG_START) + "\"" + kv.second
					+ "\" no such file or directory");
				return (EXIT_FAILURE);
			}
			if (S_ISDIR(buff.st_mode)) {
				printError(std::string(ERR_MSG_START) + "\"" + kv.second
				  + "\" is a directory");
				return (EXIT_FAILURE);
			}
		}
		if (!conf.port) {
			printError(std::string(ERR_MSG_START) + "invalid port \""
				+ std::to_string(conf.port) + "\"");
			return (EXIT_FAILURE);
		}
	}
	size = serverConfigs.size();
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = i+1; j < size; ++j) {
			if (serverConfigs[i].port == serverConfigs[j].port
					&& serverConfigs[i].ip == serverConfigs[j].ip) {
				printError(std::string(ERR_MSG_START) + "duplicate address and port pair \""
					+ IPtoString(serverConfigs[i].ip)
					+ " : " + std::to_string(serverConfigs[i].port) + "\"");
				return (EXIT_FAILURE);
	 		}
		}
	}
	return (EXIT_SUCCESS);
}
