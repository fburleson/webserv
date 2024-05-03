/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 18:10:28 by bjacobs           #+#    #+#             */
/*   Updated: 2024/05/03 17:37:33 by bjacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

#define ERR_MSG_START	"webserv: cgi: "

class CgiHandler {
	private:
		CgiHandler(void);

		std::map<std::string, std::string>	_env;
		std::string							_output;
		std::string							_body;

		void	_init(const t_httprequest &request, const t_route &route);
		void	_execute_cgi(const int &inFD, const int &outFD) const;
		void	_deleteCharEnv(char **envp) const;
		char	**_envToChar(void) const;

	public:
		CgiHandler(const t_httprequest &request, const t_route &route, const std::vector<std::byte> &body);

		bool		startExecution(void);

		std::string	getOutput(void) const;
};

