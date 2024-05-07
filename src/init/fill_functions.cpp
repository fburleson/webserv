/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_functions.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:09:32 by bjacobs           #+#    #+#             */
/*   Updated: 2024/05/06 16:41:33 by bjacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init.hpp"
#include <cstring>

static void	printBadAliasKey(const std::string &keyString,
		const std::string::iterator &value) {
	std::cerr << WRN_MSG_START << "unable to assign "
			  << "\"" << std::string(value, skipWord(value)) << "\""
			  << " to " << keyString
			  << " in alias; ignoring..." << std::endl;
}

void	fillAlias(std::string::iterator& word, const uint8_t keyWordIndex, t_conf &conf) {
	switch (keyWordIndex) {
		case LISTEN:
			printBadAliasKey("port", word);
			break;
		case SERVER_NAMES:
			printBadAliasKey("server_name", word);
			break;
		case ROOT:
			conf.default_route.root = getWordValue(word);
			parseLeftOverString(word, "root");
			break;
		case AUTOINDEX:
			conf.default_route.autoindex = getAutoIndex(word);
			parseLeftOverString(word, "autoindex");
			break;
		case IP:
			printBadAliasKey("ip", word);
			break;
		case ALLOWED:
			 addMethods(word, conf.default_route.allowed_methods);
			 break;
		case MAX_BODY_SIZE:
			printBadAliasKey("max_body_size", word);
			break;
		case ERR_PAGE:
			printBadAliasKey("err_page", word);
			break;
		default:
			printBadAliasKey("index", word);
	}
}

void	fillConfig(std::string::iterator& word, const uint8_t keyWordIndex, t_conf &conf) {
	switch (keyWordIndex) {
		case LISTEN:
			conf.port = getNumberValue(word);
			parseLeftOverString(word, "listen");
			break;
		case SERVER_NAMES:
			fillServerNames(word, conf.server_names);
			break;
		case ROOT:
			conf.default_route.root = getWordValue(word);
			parseLeftOverString(word, "root");
			break;
		case AUTOINDEX:
			conf.default_route.autoindex = getAutoIndex(word);
			parseLeftOverString(word, "autoindex");
			break;
		case IP:
			conf.ip = getIP(word);
			break;
		case ALLOWED:
			 addMethods(word, conf.default_route.allowed_methods);
			 break;
		case MAX_BODY_SIZE:
			 conf.max_body_size = getNumberValue(word);
			 parseLeftOverString(word, "max_body_size");
			 break;
		case ERR_PAGE:
			 addErrPage(word, conf.err_pages);
			 parseLeftOverString(word, "err_page");
			 break;
		case INDEX:
			conf.default_route.index = getWordValue(word);
			parseLeftOverString(word, "index");
			break;
	}
}

void	fillServerNames(std::string::iterator &start, std::vector<std::string> &serverNames) {
	std::string::iterator end;

	for (end = start; *start && *start != '}'; end = start) {
		while (*end && !std::isspace(*end) && *end != '}') {
			if (std::strchr("{", *end) && (end == start || *(end-1) == '\\'))
				throw ConfigException(std::string(ERR_OPEN_BRACKET) + " \"" + &(*start) + "\"");
			++end;
		}
		serverNames.push_back(std::string(start, end));
		start = getNextWord(end);
	}
}

void	addErrPage(std::string::iterator &end, std::map<HTTPStatus, std::string> &err_pages) {
	unsigned int			code;
	std::string				path;
	std::string::iterator	start;

	code = getNumberValue(end);
	start = end;
	for (end = getNextWord(start); *end && !std::isspace(*end) && *end != '}'; ++end) {
		if (*end == '{')
			throw ConfigException(std::string(ERR_OPEN_BRACKET) + " \"" + &(*start) + "\"");
	}
	path = std::string(start, end);
	err_pages.insert({static_cast<HTTPStatus>(code), path});
}

void	addMethods(std::string::iterator &start,  std::set<HTTPMethod> &allowed_methods) {
	allowed_methods.insert(getMethodEnum(&(*start)));
	while (*getNextWord(start) && *start != '}') {
		allowed_methods.insert(getMethodEnum(&(*start)));
	}
}
