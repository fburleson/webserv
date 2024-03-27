#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define	OK		0
#define	ERR_NO_CONFIG	2
#define	ERR_NOP_SOCK	3

#define	SOCK_READ_SIZE	1024
#define	SOCK_QUEUE_SIZE	10
#define	POLL_TIMEOUT	100


typedef struct	s_ip
{
	uint8_t	a;
	uint8_t	b;
	uint8_t	c;
	uint8_t	d;
}		t_ip;

//	socket.cpp

pollfd	open_listen_socket(const uint32_t &ip, const uint16_t &port);
pollfd	open_connection_socket(int socket_fd);

//	UTIL

int		error(const std::string &msg, int err_code);
uint32_t	ip(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
