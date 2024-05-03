/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 21:39:39 by bjacobs           #+#    #+#             */
/*   Updated: 2024/05/03 19:38:51 by bjacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initialization.hpp"
#include <cctype>
#include <fstream>

static void	findOpenScope(std::ifstream &configFile, std::string::iterator &word, std::string::iterator &wordEnd) {
	std::string	line;

	if (*wordEnd == '{')
		word = wordEnd;
	else if (*getNextWord(word) && *word != '{')  {
		throw ConfigException(std::string("expected \'{\' got \"") + &(*word) + "\"");
	}
	while (*word != '{' && std::getline(configFile, line)) {
		if ((word = getFirstWord(line.begin())) == line.end())
			continue;
	}
	if (*word != '{')
		throw ConfigException(std::string("expected \'{\' got \"") + &(*word) + "\"");
}

static std::string	createAlias(std::ifstream &configFile, t_conf &conf, std::string::iterator word);
static std::string	handleScope(std::ifstream &configFile, t_conf &conf, 
		void (*fillFunction)(std::string::iterator&, const uint8_t, t_conf &)) {
	uint8_t					keyWordIndex;
	std::string				line;
	std::string::iterator	word;

	while (std::getline(configFile, line)) {
		if ((word = getFirstWord(line.begin())) == line.end())
			continue;
		if (*word == '}')
			break;
		while (*word == '/') {
			line = createAlias(configFile, conf, word);
			word = line.begin() + line.find('}');
			if (*(++word) && std::isspace(*word))
				getNextWord(word);
		}
		if (!*word)
			continue;
		keyWordIndex = getKeywordIndex(word);
		parseSeparator(word);
		fillFunction(word, keyWordIndex, conf);
		if (*word == '}')
			break;
	}
	if (word == line.end() || *word != '}')
		throw ConfigException(ERR_SCOPE_END);
	return (line);
}

static std::string	createAlias(std::ifstream &configFile, t_conf &conf, std::string::iterator word) {
	t_conf					newAlias;
	uint8_t					keyWordIndex;
	std::string				lastLine;
	std::string::iterator	wordEnd;
	
	if (!conf.default_route.alias.empty())
		throw ConfigException(std::string(ERR_MSG_START) + "alias defined in an alias");
	for (wordEnd = word; !std::isspace(*wordEnd) && *(wordEnd+1); ++wordEnd) {
		if (*wordEnd == '{' && *(wordEnd-1) != '\\'
				&& !std::isspace(*(wordEnd+1)))
			throw  ConfigException(std::string(ERR_MSG_START) + "\"{\" in alias directive");
	}
	newAlias.default_route.alias = std::string(word, wordEnd);
	newAlias.default_route.autoindex = DEFAULT_AUTOINDEX;
	findOpenScope(configFile, word, wordEnd);
	if ((!std::isspace(*(++word)) && *word) || *getNextWord(word)) {
		keyWordIndex = getKeywordIndex(word);
		parseSeparator(word);
		fillAlias(word, keyWordIndex, newAlias);
	}
	lastLine = handleScope(configFile, newAlias, fillAlias);
	conf.routes.push_back(newAlias.default_route);
	return (lastLine);
}

bool	initConfig(const std::string &filePath, std::vector<t_conf> &serverConfigs) {
	std::string		abyss;
	std::ifstream	configFile(filePath);

	if (!configFile.is_open()) {
		printError(std::string(ERR_MSG_START) + "no such config file");
		return (false);
	}
	while (std::getline(configFile, abyss, '{') && !configFile.eof()) {
		t_conf	conf;
		setDefault(conf);
		try {
			handleScope(configFile, conf, fillConfig);
		}
		catch (const ConfigException &e) {
			printError(e.what());
			return (false);
		}
		serverConfigs.push_back(conf);
	}
	if (parseConfigs(serverConfigs) != EXIT_SUCCESS)
		return (true); // turn to false!!!
	return (true);
}
