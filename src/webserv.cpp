#include "webserv.hpp"
#include "VHost.hpp"
#include "Server.hpp"

int	main(int argc, char **argv)
{
	std::vector<t_conf>	configs;
	Server			server;
	int			init_status;
	std::string		buffer;
	t_httprequest		request;
	t_httpresponse		response;

	if (argc <= 1)
		return (error("no configuration file provided", ERR_NO_CONFIG));
	if (!initConfig(argv[1], configs))
			return (EXIT_FAILURE);
	init_status = server.init(configs);
	if (server.init(configs) != OK)
		return (error("port conflict in configuration", init_status));
	while (true)
	{
		server.poll_events();
		for (const t_socket &socket : server.get_sockets())
		{
			if (socket.type == LISTEN && has_socket_recv(socket))
				server.add_connection(socket);
			else if (socket.type == CONNECTION && server.has_response(socket) && can_socket_send(socket))
				server.send_queued_response(socket);
			else if (socket.type == CONNECTION && has_socket_recv(socket))
			{
				buffer = read_file(socket.poll_fd.fd);
				if (buffer.empty())
				{
					server.close_socket(socket);
					continue ;
				}
				request = parse_request(buffer);
				request.client = socket.poll_fd;
				response = server.process_request(request, socket);
				server.add_response(socket, response);
			}
		}
	}
	return (OK);
}

