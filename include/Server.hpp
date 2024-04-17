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
		std::vector<t_socket>				_sockets;
		std::map<uint16_t, std::vector<VHost>>		_vhosts;
		std::map<int, std::queue<t_httpresponse>>	_responses;

		VHost						_pick_vhost(const t_httprequest &request, const std::vector<VHost> &vhosts) const;

	public:
		Server(void);
		~Server(void);

		int						init(const std::vector<t_conf> &confs);
		void						close_socket(const t_socket &socket);
		void						add_socket(const uint32_t &ip, const uint16_t &port);
		void						add_connection(const t_socket &socket);
		void						add_vhost(const uint16_t &port, const VHost &vhost);
		void						add_response(const t_socket &socket, const t_httpresponse &response);
		std::vector<t_socket>				get_sockets(void) const;
		bool						has_response(const t_socket &socket) const;
		void						poll_events(void);
		t_httpresponse					process_request(const t_httprequest &request, const t_socket &connection) const;
		void						send_queued_response(const t_socket &socket);
};

bool								has_socket_recv(const t_socket &socket);
bool								can_socket_send(const t_socket &socket);

