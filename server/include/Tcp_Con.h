#pragma once

#ifndef TCP_CON_H
#define TCP_CON_H

#include <vector>

class Tcp_Con
{
public:
	explicit Tcp_Con(int sock_fd = 0);
	virtual ~Tcp_Con();

	int m_sock_fd;
	std::vector<char> send_buff;
	std::vector<char> write_buff;
};

#endif