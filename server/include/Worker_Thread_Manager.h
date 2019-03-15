#pragma once

#ifndef WORKER_THREAD_MANAGER_H
#define WORKER_THREAD_MANAGER_H

#include "Worker_Thread.h"
#include <vector>

class Worker_Thread_Manager
{
public:
	explicit Worker_Thread_Manager(int thread_num = 12, int m_max_wait_time = 200, int max_queue_size = -1);
	virtual ~Worker_Thread_Manager();

	std::future<Result> dispatch_Work(std::function<Result(Helper*)> work);
private:
	std::shared_ptr<Worker_Thread> choose_Suitable_Thread();

	int m_thread_number;
	int m_max_task_queue_size;
	int m_max_wait_time;
	std::vector<std::shared_ptr<Worker_Thread>> m_thread_pool;
};

#endif