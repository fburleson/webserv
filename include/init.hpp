/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:37:25 by bjacobs           #+#    #+#             */
/*   Updated: 2024/05/22 15:20:20 by bjacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include <iostream>

#define ERR_MSG_START		"webserv: config: "
#define ERR_NUM_ARGS		ERR_MSG_START "invalid number of args in "
#define ERR_OPEN_BRACKET	ERR_MSG_START "open brace inside scope"
#define ERR_SCOPE_END		ERR_MSG_START "unclosed scope"
#define ERR_IP				ERR_MSG_START "invalid ip"
#define ERR_SEPARATOR		ERR_MSG_START "no separator \':\' found"
#define ERR_NO_VALUE		ERR_MSG_START "no value found after \':\'"
#define ERR_UNKNOWN_KEY		ERR_MSG_START "unknown key "

#define WRN_MSG_START		ERR_MSG_START "warning: "

#define DEFAULT_IP			0x7F000001 //127.0.0.1
#define DEFAULT_BODY		1024
#define DEFAULT_AUTOINDEX	false
#define DEFAULT_PORT		80

enum keyWordIndices {
	LISTEN,
	SERVER_NAMES,
	ROOT,
	AUTOINDEX,
	IP,
	ALLOWED,
	MAX_BODY_SIZE,
	ERR_PAGE,
	INDEX,
	REDIRECT
};

class ConfigException : public std::exception {
	private:
		std::string	_errorMsg;
	public:
		ConfigException(const std::string &msg)
			: _errorMsg(msg) {};

		const char	*what() const noexcept override { 
			return (_errorMsg.c_str());
		}
};

void					parseLeftOverString(std::string::iterator &start, const std::string &keyWord);
void					parseSeparator(std::string::iterator &word);
void					testConfigs(std::vector<t_conf> &serverConfigs);
bool					parsePath(const std::string &path);

uint32_t				getIP(std::string::iterator word);
uint8_t					getKeywordIndex(const std::string::iterator &wordi);
enum HTTPMethod			getMethodEnum(const std::string &method);
int						getNumberValue(std::string::iterator &start);
std::string				getWordValue(std::string::iterator &end);
bool					getAutoIndex(std::string::iterator& word);

std::string::iterator	skipWord(const std::string::iterator &it);
std::string::iterator	getFirstWord(std::string::iterator it);
std::string::iterator&	getNextWord(std::string::iterator &it);

void					addMethods(std::string::iterator &start,  std::set<HTTPMethod> &allowed_methods);
void					addErrPage(std::string::iterator &end, std::map<HTTPStatus, std::string> &err_pages);
void					fillServerNames(std::string::iterator &start, std::vector<std::string> &serverNames);
void					fillAlias(std::string::iterator& word, const uint8_t keyWordIndex, t_conf &conf);
void					fillConfig(std::string::iterator& word, const uint8_t keyWordIndex, t_conf &conf);

void					printError(const std::string &msg);
void					printRoute(const t_route &route);
void					printConfig(t_conf &config);
void					setDefault(t_conf &conf);
std::string				IPtoString(const uint32_t &ip);
