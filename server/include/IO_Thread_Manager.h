#pragma once

#ifndef IO_THREAD_MANAGER_H
#define IO_THREAD_MANAGER_H

#include "Tcp_Con.h"
#include "IO_Thread.h"
#include "Worker_Thread_Manager.h"
#include <memory>

class IO_Thread_Manager
{
public:
	IO_Thread_Manager(std::shared_ptr<Worker_Thread_Manager> worker_thread_manager, int thread_num = 12, int max_wait_time = 200, int max_con_map_size = -1);
	virtual ~IO_Thread_Manager();

	bool insert_New_Con(int sock_fd);
	bool stop();
	bool start();
	bool terminate();

	IO_Thread_Manager(const IO_Thread_Manager&) = delete;
	IO_Thread_Manager& operator= (const IO_Thread_Manager&) = delete;

private:
	std::shared_ptr<IO_Thread> choose_Suitable_Thread();
	std::shared_ptr<Worker_Thread_Manager> p_worker_thread_manager;

	int m_thread_number;
	int m_max_con_map_size;
	int m_max_wait_time;
	std::vector<std::shared_ptr<IO_Thread>> m_thread_pool;
};


#endif
