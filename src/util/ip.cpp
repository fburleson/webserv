#include "webserv.hpp"

uint32_t	ip(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	uint32_t	ip32;
	t_ip		s_ip;
	
	s_ip.byte0 = a;
	s_ip.byte1 = b;
	s_ip.byte2 = c;
	s_ip.byte3 = d;
	ip32 = *((uint32_t *)&s_ip);
	return (ip32);
}

