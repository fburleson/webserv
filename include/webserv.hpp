#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>

namespace fs = std::filesystem;

#define	OK		0
#define	ERR_NO_CONFIG	2
#define	ERR_NOP_SOCK	3
#define	ERR_NO_CONN	4
#define	ERR_NO_POLL	5

#define	SOCK_READ_SIZE	1024
#define	SOCK_QUEUE_SIZE	100
#define	POLL_TIMEOUT	100

#define	BUFF_READ_SIZE	1024

#define	HTTP_VERSION	"HTTP/1.1"

enum HTTPMethod {
	HTTP_UNKNOWN,
	HTTP_GET,
	HTTP_POST,
	HTTP_DELETE
};

enum HTTPStatus {
	HTTP_CONTINUE = 100,
	HTTP_OK = 200,
	HTTP_CREATED = 201,
	HTTP_NO_CONTENT = 204,
	HTTP_PERM_MOVE = 301,
	HTTP_NOT_FOUND = 404,
	HTTP_BAD_METHOD = 405,
	HTTP_TOO_LARGE = 413,
	HTTP_INTERNAL_ERROR = 500,
	HTTP_BAD_VERSION = 505
};

typedef struct	s_server
{
	std::vector<pollfd>			sockets;
	std::vector<size_t>			listens;
}		t_server;

typedef struct	s_route
{
	std::string				root;
	std::string				index;
	bool					autoindex;
	std::string				http_redirect;
	std::set<HTTPMethod>			allowed_methods;
}		t_route;

typedef struct	s_httprequest
{
	HTTPMethod				method;
	std::string				resource;
	std::string				version;
	std::map<std::string, std::string>	head;
	std::vector<std::byte>			body;
	pollfd					client;
}		t_httprequest;

typedef struct	s_httpresponse
{
	HTTPStatus				status;
	std::string				message;
	std::string				version;
	std::map<std::string, std::string>	head;
	std::vector<std::byte>			body;
	pollfd					client;
}		t_httpresponse;

typedef struct	s_ip
{
	uint8_t	a;
	uint8_t	b;
	uint8_t	c;
	uint8_t	d;
}		t_ip;

//	SOCKET

pollfd					open_listen_socket(const uint32_t &ip, const uint16_t &port);
pollfd					open_connection_socket(int socket_fd);

//	SERVER

void					add_socket(t_server &server, const uint32_t &ip, const uint16_t &port);
void					poll_server(t_server &server);
void					add_connection(t_server &server, const pollfd &socket, std::vector<size_t> &connections);


//	REQUEST

t_httprequest				parse_request(const std::string &request);
std::map<std::string, std::string>	parse_head(const std::string &head);

//	RESPONSE

std::vector<std::byte>			stobyte(const std::string &body);
std::vector<std::byte>			ftobyte(const std::string &file);
void					send_response(t_httpresponse &response);
std::string				process_message(const HTTPStatus &code);
std::vector<std::byte>			generate_err_page(const HTTPStatus &code);
std::vector<std::byte>			generate_dir_list(const std::string &resource, const std::string &root);
bool					is_method_allowed(const HTTPMethod &method, const t_route &route);

//	UTIL

int					error(const std::string &msg, const int &err_code);
uint32_t				ip(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
bool					path_exists(const std::string &path);

