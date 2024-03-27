#include "webserv.hpp"

uint32_t	ip(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	uint32_t	ip32;
	t_ip		s_ip;
	
	s_ip.a = a;
	s_ip.b = b;
	s_ip.c = c;
	s_ip.d = d;
	ip32 = *((uint32_t *)&s_ip);
	return (ip32);
}

