#pragma once

#ifndef LISTENER_H
#define LISTENER_H

#include "Worker_Thread_Manager.h"
#include "IO_Thread_Manager.h"

#include <memory>
#include <event2/event.h>
#include <event2/bufferevent.h>

class Listener
{
public:
	Listener(int port = 1234);
	virtual ~Listener();
	bool start();
	bool run();
	bool stop();
	Listener(const Listener&) = delete;
	Listener& operator= (const Listener&) = delete;

	bool bind_And_Listen(int sock_fd);

	void accept_Callback(evutil_socket_t sock_fd, short event, void *arg);

private:
	int m_port;
	int m_sock_fd;
	event_base* p_event_base;
	std::shared_ptr<std::thread> p_listen_thread;
	std::shared_ptr<IO_Thread_Manager> p_io_thread_manager;
	std::shared_ptr<Worker_Thread_Manager> p_worker_thread_manager;

	std::atomic<bool> m_listen_flag;

};


static void accept_Callback_Wrapper(evutil_socket_t sock_fd, short event, void *arg)
{
	reinterpret_cast<Listener*>(arg)->accept_Callback(sock_fd, event, arg);
}

#endif