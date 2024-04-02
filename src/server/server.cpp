#include "webserv.hpp"

void	add_socket(t_server &server, const uint32_t &ip, const uint16_t &port)
{
	pollfd	socket;

	socket = open_listen_socket(ip, port);
	if (socket.fd == -1)
	{
		error("failed to open server socket", ERR_NOP_SOCK);
		return ;
	}
	server.sockets.push_back(socket);
	server.listens.push_back(server.sockets.size() - 1);
}

void	poll_server(t_server &server)
{
	if (poll(&server.sockets[0], server.sockets.size(), POLL_TIMEOUT) == -1)
		std::cerr << " poll(): " << strerror(errno) << std::endl;
}

void	add_connection(t_server &server, const pollfd &socket, std::vector<size_t> &connections)
{
	pollfd	connection_socket;

	connection_socket = open_connection_socket(socket.fd);
	if (connection_socket.fd == -1)
	{
		error("failed to open connection socket", ERR_NO_CONN);
		return ;
	}
	server.sockets.push_back(connection_socket);
	connections.push_back(server.sockets.size() - 1);
}

