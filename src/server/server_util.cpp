#include "Server.hpp"

bool	has_socket_recv(const t_socket &socket)
{
	return (socket.poll_fd.revents & POLLIN);
}

bool	can_socket_send(const t_socket &socket)
{
	return (socket.poll_fd.revents & POLLOUT);
}

