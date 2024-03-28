#include "webserv.hpp"

int	main(int argc, char **argv)
{
	std::vector<pollfd>	server;
	std::vector<pollfd *>	connections;
	pollfd			listen;
	pollfd			connection;
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
		for (pollfd *&current_connection : connections)
		{
			if (current_connection == nullptr)
				continue ;
			if (current_connection->revents & POLLIN)
			{
				status = read(current_connection->fd, buffer, SOCK_READ_SIZE);
				if (status == 0)
				{
					close(current_connection->fd);
					current_connection->fd = -1;
					current_connection = nullptr;
       					std::cout << "a connection has closed." << std::endl;
				}
				else
       					std::cout << buffer << "\n" << std::endl; 
				buffer[0] = '\n';
			}
		}
	}
	return (OK);
}

