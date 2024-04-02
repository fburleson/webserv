#pragma once
#include "webserv.hpp"

class	VHost
{
	private:
		std::string	_root;

		VHost(const VHost &host);

		VHost	&operator=(const VHost &host);

	public:
		VHost(void);
		~VHost(void);

		void		set_root(const std::string &root);
		t_httpresponse	process_request(const t_httprequest &request) const;
};

