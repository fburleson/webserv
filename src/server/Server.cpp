#include "Server.hpp"

Server::Server(void)
{}

Server::~Server(void)
{}

void	Server::init(const std::vector<t_conf> &confs)
{
	VHost	vhost;

	for (const t_conf &conf : confs)
	{
		if (this->_vhosts.find(conf.port) == this->_vhosts.end())
			this->add_socket(conf.ip, conf.port);
		vhost.init(conf);
		this->add_vhost(conf.port, vhost);
	}
}

void	Server::close_socket(const t_socketref &ref)
{
	pollfd	socket = this->get_socket(ref);

	close(socket.fd);
	this->_sockets[ref.idx].fd = -1;
	std::cout << "a client has diconnected." << std::endl;
}

void	Server::add_socket(const uint32_t &ip, const uint16_t &port)
{
	pollfd		socket;
	t_socketref	reference;

	socket = open_listen_socket(ip, port);
	if (socket.fd == -1)
	{
		error("failed to open server socket", ERR_NOP_SOCK);
		return ;
	}
	this->_sockets.push_back(socket);
	reference.port = port;
	reference.idx = this->_sockets.size() - 1;
	this->_listening.push_back(reference);
}

void	Server::add_connection(const t_socketref &socket)
{
	pollfd		connection_socket;
	t_socketref	reference;

	connection_socket = open_connection_socket(this->get_socket(socket).fd);
	if (connection_socket.fd == -1)
	{
		error("failed to open connection socket", ERR_NO_CONN);
		return ;
	}
	this->_sockets.push_back(connection_socket);
	reference.port = socket.port;
	reference.idx = this->_sockets.size() - 1;
	this->_connections.push_back(reference);
}

void	Server::add_vhost(const uint16_t &port, const VHost &vhost)
{
	this->_vhosts[port].push_back(vhost);
}

void	Server::poll_events(void) const
{
	if (poll((pollfd *)&this->_sockets[0], this->_sockets.size(), POLL_TIMEOUT) == -1)
		std::cerr << " poll(): " << strerror(errno) << std::endl;
}

bool	Server::has_socket_recv(const t_socketref &ref) const
{
	const pollfd &socket = this->get_socket(ref);

	return (socket.revents & POLLIN);
}

std::vector<t_socketref>	Server::get_listen_sockets(void) const
{
	return (this->_listening);
}

std::vector<t_socketref>	Server::get_connection_sockets(void) const
{
	return (this->_connections);
}

pollfd				Server::get_socket(const t_socketref &ref) const
{
	return (this->_sockets[ref.idx]);
}

t_httpresponse	Server::process_request(const t_httprequest &request, const t_socketref &connection) const
{
	t_httpresponse	response;
	VHost		vhost = this->_fallback_vhost;

	if (this->_vhosts.find(connection.port) != this->_vhosts.end())
		vhost = this->_vhosts.at(connection.port)[0];
	response = vhost.process_request(request);
	return (response);
}

