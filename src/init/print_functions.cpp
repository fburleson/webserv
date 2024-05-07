/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_functions.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:17:46 by bjacobs           #+#    #+#             */
/*   Updated: 2024/05/06 16:43:02 by bjacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init.hpp"

void	printError(const std::string &msg) {
	std::cerr << msg << std::endl;
}

void	printRoute(const t_route &route) {
	if (route.alias.empty())
		std::cout << "Default Route {" << std::endl;
	else
		std::cout << route.alias << " {" << std::endl;
	std::cout << "\tautoindex = " << route.autoindex << std::endl
			  << "\troot = " << route.root << std::endl
			  << "\tAllowed methods =";
	for (const HTTPMethod &val : route.allowed_methods)
		std::cout << " " << val;
	std::cout << std::endl << "}"  << std::endl;
}

void	printConfig(t_conf &config) {
	std::vector<std::string>::iterator	it, ite;
	std::set<HTTPMethod>::iterator		it2, ite2;

	std::cout << "ip = " << IPtoString(config.ip) << std::endl
			  << "port = " << config.port << std::endl
			  << "max_body_size = " << config.max_body_size << std::endl
			  << "index = " << config.default_route.index << std::endl;
	ite = config.server_names.end();
	std::cout << "server_names =";
	for (const std::string &names : config.server_names)
		std::cout << " " << names;
	std::cout << std::endl;
	for (const std::pair<const HTTPStatus, std::string> &kv : config.err_pages)
		std::cout << "code: " << kv.first << "; page: " << kv.second << std::endl;
	printRoute(config.default_route);
	for (const t_route &val : config.routes)
		printRoute(val);
}
