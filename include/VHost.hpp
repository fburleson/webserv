#pragma once
#include "webserv.hpp"

class	VHost
{
	private:
		std::string				_root;
		std::map<HTTPStatus, std::string>	_err_pages;

		VHost(const VHost &host);

		VHost	&operator=(const VHost &host);

		std::string		_parse_resource(const std::string &resource) const;
		HTTPStatus		_process_code(const t_httprequest &request) const;
		std::vector<std::byte>	_process_body(const t_httprequest &request, const t_httpresponse &response) const;

	public:
		VHost(void);
		~VHost(void);

		void		set_root(const std::string &root);
		void		set_err_page(const HTTPStatus &code, const std::string &file);
		t_httpresponse	process_request(const t_httprequest &request) const;
};

