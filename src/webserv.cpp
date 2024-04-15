#include "webserv.hpp"
#include "VHost.hpp"
#include "Server.hpp"

int	main(int argc, char **argv)
{
	Server		server;
	std::string	buffer;
	t_httprequest	request;
	VHost		virtual_host;
	t_httpresponse	response;

	argv = argv;
	if (argc <= 1)
		return (error("no configuration file provided", ERR_NO_CONFIG));
	server.add_socket(0, 9999);
	server.add_socket(0, 8888);
	virtual_host.set_root("/home/jburleson/documents/codam/webserv_website/");
	virtual_host.allow_method(HTTP_GET);
	server.add_vhost(9999, virtual_host);
	while (true)
	{
		server.poll_events();
		for (const t_socketref &socket : server.get_listen_sockets())
		{
			if (server.has_socket_recv(socket))
				server.add_connection(socket);
		}
		for (const t_socketref &connection : server.get_connection_sockets())
		{
			if (server.has_socket_recv(connection))
			{
				buffer = read_file(server.get_socket(connection).fd);
				if (buffer.empty())
				{
					server.close_socket(connection);
					continue ;
				}
				request = parse_request(buffer);
				request.client = server.get_socket(connection);
				response = server.process_request(request, connection);
				send_response(response);
				buffer.clear();
			}
		}
	}
}

