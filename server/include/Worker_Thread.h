#pragma once

#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include "Thread_Base.h"
#include <future>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>


/////////////////////////////////////////////
///implement Result and Helper for specific task

typedef std::chrono::time_point<std::chrono::high_resolution_clock, \
	std::chrono::milliseconds> time_point;

class Result
{
public:
	inline explicit Result()
	{
		m_worker_id = std::this_thread::get_id();
	}
	inline ~Result(){};
	std::thread::id m_worker_id;
};


class Helper
{
public:
	inline explicit Helper(){};
	inline ~Helper() {};
};


class Task
{
public:
	explicit Task() {};
	virtual ~Task() {};
	virtual Result exec(Helper*) { Result t; return t; };
	std::promise<Result> m_promise;
};


class Worker_Thread :public Thread_Base
{
public:
	explicit Worker_Thread(int max_queue_size = -1, int max_wait_time = 200);
	virtual ~Worker_Thread();

	bool do_Work();
	std::future<Result> insert_Task(std::shared_ptr<Task>);
	int get_Task_Queue_Size();
	std::thread::id get_Thread_Id();

	std::condition_variable m_cv;
	std::mutex m_cv_mutex;


private:
	void run();

	std::queue<std::pair<std::shared_ptr<Task>, time_point>> m_task_queue;
	std::mutex m_queue_mutex;
	
	std::shared_ptr<std::thread> p_thread;

	std::shared_ptr<Helper> p_helper;

	int m_max_queue_size;
	int m_max_wait_time;
	std::atomic<int> m_thread_status;
	std::atomic<bool> m_empty_flag;
	std::thread::id m_thread_id;
};

#endif
