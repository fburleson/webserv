#include "webserv.hpp"

int	main(int argc, char **argv)
{
	std::vector<pollfd>	server;
	std::vector<pollfd *>	connections;
	pollfd			listen;
	pollfd			connection;
	pollfd			*current_connection;
	t_httprequest		request;
	char			buffer[SOCK_READ_SIZE];	
	int			status;

	argv = argv;
	if (argc <= 1)
		return (error("no configuration file", ERR_NO_CONFIG));
	listen = open_listen_socket(ip(127, 0, 0, 1), 9999);
	if (listen.fd == -1)
		return (error("failed to start server", ERR_NOP_SOCK));
	server.push_back(listen);
	while (true)
	{
		poll(&server[0], server.size(), POLL_TIMEOUT);
		if (server[0].revents & POLLIN)
		{
			connection = open_connection_socket(server[0].fd);
			if (connection.fd == -1)
				error("failed to accept connection", ERR_NO_CONN);
			else
			{
				server.push_back(connection);
				connections.push_back(&server.back());
			}
		}
		for (size_t i = 0; i < connections.size(); i++)
		{
       			current_connection = connections[i];
			if (current_connection->fd == -1)
			{
				connections.erase(connections.begin() + i);
				if (i < connections.size())
					current_connection = connections[i];
				else
					break ;
			}
			if (current_connection->revents & POLLIN)
			{
				memset(buffer, 0, sizeof(char) * SOCK_READ_SIZE);
				status = read(current_connection->fd, buffer, SOCK_READ_SIZE);
				if (status == -1)
				{
					std::cout << "read(): " << strerror(errno) << std::endl;
					continue ;
				}
				if (status == 0)
				{
					close(current_connection->fd);
					current_connection->fd = -1;
       					std::cout << "a connection has closed." << std::endl;
				}
				else
				{
					request = parse_request(buffer);
					std::cout << "resource: " << request.resource << std::endl;
       					std::cout << "version:	" << request.version << std::endl;
					for (auto const &[key, val] : request.head)
						std::cout << key << ": " << val << std::endl;
       					std::cout << std::endl;
					for (const std::byte &byte : request.body)
						std::cout << (char)byte;
       					std::cout << std::endl;
				}
			}
		}
	}
	return (OK);
}

