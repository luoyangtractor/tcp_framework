#pragma once

#ifndef TCP_SESSION_H
#define TCP_SESSION_H

#include <vector>

class Tcp_Session
{
public:
	Tcp_Session();
	virtual ~Tcp_Session();
private:

	std::vector<char> send_buff;
	std::vector<char> write_buff;
};

#endif