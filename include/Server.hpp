#pragma once
#include "webserv.hpp"
#include "VHost.hpp"

typedef struct	s_socketref
{
	uint16_t	port;
	size_t		idx;
}		t_socketref;

class	Server
{
	private:
		std::vector<pollfd>			_sockets;
		std::vector<t_socketref>		_listening;
		std::vector<t_socketref>		_connections;
		VHost					_fallback_vhost;
		std::map<uint16_t, std::vector<VHost>>	_vhosts;

	public:
		Server(void);
		~Server(void);

		void					close_socket(const t_socketref &ref);
		void					add_socket(const uint32_t &ip, const uint16_t &port);
		void					add_connection(const t_socketref &socket);
		void					add_vhost(const uint16_t &port, const VHost &vhost);
		std::vector<t_socketref>		get_listen_sockets(void) const;
		std::vector<t_socketref>		get_connection_sockets(void) const;
		pollfd					get_socket(const t_socketref &ref) const;
		void					poll_events(void) const;
		bool					has_socket_recv(const t_socketref &ref) const;
		t_httpresponse				process_request(const t_httprequest &request, const t_socketref &connection) const;
};

