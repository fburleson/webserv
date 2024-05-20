/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 22:28:48 by bjacobs           #+#    #+#             */
/*   Updated: 2024/05/20 14:53:52 by fsarkoh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init.hpp"
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

void	testConfigs(std::vector<t_conf> &serverConfigs) {
	struct stat	buff;

	for (t_conf conf : serverConfigs) {
		for (const std::pair<const HTTPStatus, std::string> &kv : conf.err_pages) {
			if (!parsePath(kv.second, &buff)) {
				printError(std::string(ERR_MSG_START) + "\"" + kv.second
					+ "\" no such file or directory");
			}
			if (S_ISDIR(buff.st_mode)) {
				printError(std::string(ERR_MSG_START) + "\"" + kv.second
				  + "\" is a directory");
			}
		}
		if (!conf.port) {
			printError(std::string(ERR_MSG_START) + "invalid port \""
				+ std::to_string(conf.port) + "\"");
		}
	}
}
