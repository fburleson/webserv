#include "webserv.hpp"

int	main(int argc, char **argv)
{
	std::vector<pollfd>		server;
	std::vector<unsigned int>	connections;
	pollfd				listen;
	pollfd				connection;
	unsigned int			current_connection;
	char				buffer[SOCK_READ_SIZE];	
	int				status;

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
			if (connection.fd)
			{
				connections.push_back(server.size());
				server.push_back(connection);
			}
		}
		for (size_t i = 0; i < connections.size(); i++)
		{
			current_connection = connections[i];
			if (server[current_connection].revents & POLLIN)
			{
				status = read(server[current_connection].fd, buffer, SOCK_READ_SIZE);
				if (status == 0)
				{
					close(server[current_connection].fd);
					server.erase(server.begin() + current_connection);
					connections.erase(connections.begin() + i);
					std::cout << "a connection has closed" << std::endl;
				}
       				else
					std::cout << buffer << "\n" << std::endl;
				buffer[0] = '\n';
			}
		}
	}
	return (OK);
}

