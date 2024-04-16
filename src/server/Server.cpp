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

void	Server::close_socket(const t_socket &socket)
{
	close(socket.poll_fd.fd);
	for (t_socket &eq_socket : this->_sockets)
	{
		if (eq_socket.poll_fd.fd == socket.poll_fd.fd)
			eq_socket.poll_fd.fd = -1;
	}
	std::cout << "a client has diconnected." << std::endl;
}

void	Server::add_socket(const uint32_t &ip, const uint16_t &port)
{
	t_socket	socket;
	pollfd		poll_fd;

	poll_fd = open_listen_socket(ip, port);
	if (poll_fd.fd == -1)
	{
		error("failed to open server socket", ERR_NOP_SOCK);
		return ;
	}
	socket.poll_fd = poll_fd;
	socket.type = LISTEN;
	socket.port = port;
	this->_sockets.push_back(socket);
}

void	Server::add_connection(const t_socket &socket)
{
	t_socket	connection_socket;
	pollfd		poll_fd;

	poll_fd = open_connection_socket(socket.poll_fd.fd);
	if (poll_fd.fd == -1)
	{
		error("failed to open connection socket", ERR_NO_CONN);
		return ;
	}
	connection_socket.poll_fd = poll_fd;
	connection_socket.type = CONNECTION;
	connection_socket.port = socket.port;
	this->_sockets.push_back(connection_socket);
}

void	Server::add_vhost(const uint16_t &port, const VHost &vhost)
{
	this->_vhosts[port].push_back(vhost);
}

std::vector<t_socket>	Server::get_sockets(void) const
{
	return (this->_sockets);
}

void	Server::poll_events(void)
{
	std::vector<pollfd>	poll_fds;

	for (const t_socket &socket : this->_sockets)
		poll_fds.push_back(socket.poll_fd);
	if (poll((pollfd *)&poll_fds[0], this->_sockets.size(), POLL_TIMEOUT) == -1)
		std::cerr << " poll(): " << strerror(errno) << std::endl;
	for (size_t i = 0; i < poll_fds.size(); i++)
		this->_sockets.at(i).poll_fd = poll_fds.at(i);
}

bool	Server::has_socket_recv(const t_socket &socket) const
{
	return (socket.poll_fd.revents & POLLIN);
}

VHost	Server::_pick_vhost(const t_httprequest &request, const std::vector<VHost> &vhosts) const
{
	VHost		vhost = vhosts[0];
	std::string	host;

	host = request.head.at("Host");
	host = host.substr(0, host.find(':'));
	for (const VHost &vhost : vhosts)
	{
		for (const std::string &name : vhost.get_server_names())
		{
			if (name == host)
				return (vhost);
		}
	}
	return (vhost);
}

t_httpresponse	Server::process_request(const t_httprequest &request, const t_socket &connection) const
{
	t_httpresponse	response;
	VHost		vhost = this->_fallback_vhost;

	if (this->_vhosts.find(connection.port) != this->_vhosts.end())
		vhost = this->_pick_vhost(request, this->_vhosts.at(connection.port));
	response = vhost.process_request(request);
	return (response);
}

