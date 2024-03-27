#include "webserv.hpp"

static int	setsockopts(int fd)
{
	int	value = 1;

	return (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)));
}

static int	bind_sock(int fd, const uint32_t &ip, const uint16_t &port)
{
	sockaddr_in	socket_address;

	socket_address.sin_family = AF_INET;
	socket_address.sin_addr.s_addr = ip;
	socket_address.sin_port = htons(port);
	return (bind(fd, (sockaddr *)&socket_address, sizeof(socket_address)));
}

pollfd	open_listen_socket(const uint32_t &ip, const uint16_t &port)
{
	pollfd	poll_fd;
	int	fd;

	poll_fd.fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		std::cerr << " socket(): " << strerror(errno) << std::endl;
		return (poll_fd);
	}
	fcntl(fd, F_SETFL, O_NONBLOCK);	
	if (setsockopts(fd) == -1)
	{
		std::cerr << " setsockopts(): " << strerror(errno) << std::endl;
		return (poll_fd);
	}
	if (bind_sock(fd, ip, port) == -1)
	{
		std::cerr << " bind(): " << strerror(errno) << std::endl;
		return (poll_fd);
	}
	if (listen(fd, SOCK_QUEUE_SIZE) == -1)
	{
		std::cerr << " listen(): " << strerror(errno) << std::endl;
		return (poll_fd);
	}
	std::cout << "opened socket on port " << port << std::endl;
	poll_fd.fd = fd;
	poll_fd.events = POLLIN;
	return (poll_fd);
}

pollfd	open_connection_socket(int socket_fd)
{
	pollfd		poll_fd;
	int		fd;
	sockaddr_in	connection_address;
	sockaddr_in	socket_address;
	size_t		connection_len;
	size_t		socket_len;

	poll_fd.fd = -1;
	connection_len = sizeof(connection_address);
	fd = accept(socket_fd, (sockaddr *)&connection_address, (socklen_t *)&connection_len);
	if (fd == -1)
	{
		std::cerr << " accept(): " << strerror(errno) << std::endl;
		return (poll_fd);
	}
	fcntl(fd, F_SETFL, O_NONBLOCK);
	socket_len = sizeof(socket_address);
	getsockname(socket_fd, (sockaddr *)&socket_address, (socklen_t *)&socket_len);
	std::cout << "accepted connection on port " << ntohs(socket_address.sin_port) << std::endl;
	poll_fd.fd = fd;
	poll_fd.events = POLLIN | POLLOUT;
	return (poll_fd);
}

