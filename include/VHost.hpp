#pragma once
#include "webserv.hpp"

class	VHost
{
	private:
		t_route					_default_route;
		std::vector<t_route>			_routes;
		std::map<HTTPStatus, std::string>	_err_pages;
		size_t					_max_body_size;

		VHost(const VHost &host);

		VHost	&operator=(const VHost &host);

		std::vector<std::byte>	_get_err_page(const HTTPStatus &code) const;
		t_route			_get_route(const t_httprequest &request) const;

		bool			_is_too_large(const t_httprequest &request) const;
		bool			_is_incomplete_dir(const t_httprequest &request, const t_route &route) const;

		t_httpresponse		_process_error(const HTTPStatus &code) const;
		t_httpresponse		_process_bad_method(const HTTPStatus &code, const t_route &route) const;
		t_httpresponse		_process_get_method(const t_httprequest &request, const t_route &route) const;
		t_httpresponse		_process_post_method(const t_httprequest &request, const t_route &route) const;
		t_httpresponse		_process_delete_method(const t_httprequest &request, const t_route &route) const;

	public:
		VHost(void);
		~VHost(void);

		void			set_root(const std::string &root);
		void			set_index(const std::string &index);
		void			set_autoindex(const bool &autoindex);
		void			set_redirect(const std::string &url);
		void			allow_method(const HTTPMethod &method);
		void			add_route(const t_route &route);
		void			set_err_page(const HTTPStatus &code, const std::string &file);
		void			set_max_body_size(const size_t &max_body_size);
		t_httpresponse		process_request(const t_httprequest &request) const;
};

