#pragma once
#include "webserv.hpp"
#include "VHost.hpp"

enum	SocketType
{
	LISTEN,
	CONNECTION
};

typedef struct	s_socket
{
	pollfd		poll_fd;
	SocketType	type;
	uint16_t	port;
}		t_socket;

class	Server
{
	private:
		std::vector<t_socket>			_sockets;
		VHost					_fallback_vhost;
		std::map<uint16_t, std::vector<VHost>>	_vhosts;

		VHost					_pick_vhost(const t_httprequest &request, const std::vector<VHost> &vhosts) const;

	public:
		Server(void);
		~Server(void);

		void					init(const std::vector<t_conf> &confs);
		void					close_socket(const t_socket &socket);
		void					add_socket(const uint32_t &ip, const uint16_t &port);
		void					add_connection(const t_socket &socket);
		void					add_vhost(const uint16_t &port, const VHost &vhost);
		std::vector<t_socket>			get_sockets(void) const;
		void					poll_events(void);
		bool					has_socket_recv(const t_socket &socket) const;
		t_httpresponse				process_request(const t_httprequest &request, const t_socket &connection) const;
};

