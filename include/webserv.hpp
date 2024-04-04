#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
// #include "VHost.hpp"

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
	HTTP_NOT_FOUND = 404,
	HTTP_BAD_METHOD = 405,
	HTTP_TOO_LARGE = 413,
	HTTP_INTERNAL_ERROR = 500,
	HTTP_BAD_VERSION = 505
};

typedef struct	s_server
{
	std::vector<pollfd>	sockets;
	std::vector<size_t>	listens;
}		t_server;

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

//	socket.cpp

pollfd					open_listen_socket(const uint32_t &ip, const uint16_t &port);
pollfd					open_connection_socket(int socket_fd);

//	SERVER

void	add_socket(t_server &server, const uint32_t &ip, const uint16_t &port);
void	poll_server(t_server &server);
void	add_connection(t_server &server, const pollfd &socket, std::vector<size_t> &connections);


//	request.cpp

t_httprequest				parse_request(const std::string &request);
std::map<std::string, std::string>	parse_head(const std::string &head);

//	REPONSE

std::vector<std::byte>	stobyte(const std::string &body);
std::vector<std::byte>	ftobyte(const std::string &file);
void			send_response(t_httpresponse &http_response);
std::string		process_message(const HTTPStatus &code);
std::vector<std::byte>	generate_err_page(const HTTPStatus &code);

//	UTIL

int					error(const std::string &msg, const int &err_code);
uint32_t				ip(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
bool					file_exists(const std::string &path);

