#pragma once
#include "webserv.hpp"

class	VHost
{
	private:
		t_route					_route;
		std::map<HTTPStatus, std::string>	_err_pages;
		size_t					_max_body_size;

		VHost(const VHost &host);

		VHost	&operator=(const VHost &host);

		std::string		_parse_resource(const std::string &resource) const;
		std::vector<std::byte>	_get_err_page(const HTTPStatus &code) const;
		void			_process_get_method(const t_httprequest &request, t_httpresponse &response) const;
		void			_process_post_method(const t_httprequest &request, t_httpresponse &response) const;
		void			_process_delete_method(const t_httprequest &request, t_httpresponse &response) const;

	public:
		VHost(void);
		~VHost(void);

		void		set_root(const std::string &root);
		void		set_err_page(const HTTPStatus &code, const std::string &file);
		void		set_max_body_size(const size_t &max_body_size);
		t_httpresponse	process_request(const t_httprequest &request) const;
};

