#pragma once

#ifndef IO_THREAD_H
#define IO_THREAD_H

#include "Thread_Base.h"
#include "Tcp_Con.h"
#include "Worker_Thread_Manager.h"
#include <event2/event.h>
#include <event2/bufferevent.h>

#include <thread>
#include <mutex>
#include <unordered_map>




class IO_Thread :public Thread_Base
{
public:
	explicit IO_Thread(std::shared_ptr<Worker_Thread_Manager> worker_thread_manager, int max_con_num = 200, int time_out = 200);
	virtual ~IO_Thread();

	bool do_Work();
	std::thread::id get_Thread_Id();
	int get_Con_Map_Size();
	bool add_Con(int);
	bool remove_Con(int);

	void read_Callback(bufferevent *bev, void *arg);

	void write_Callback(bufferevent *bev, void *arg);

	void error_Callback(bufferevent *bev, short event, void *arg);

private:
	void run();

	std::unordered_map<int, std::shared_ptr<Tcp_Con>> m_con_map;
	std::mutex m_con_map_mutex;

	std::shared_ptr<std::thread> p_thread;
	std::shared_ptr<Worker_Thread_Manager> p_worker_thread_manager;
	event_base* p_event_base;
	int m_max_con_num;
	int m_time_out;

	std::atomic<int> m_thread_status;
	std::thread::id m_thread_id;


	
};

static void read_Callback_Wrapper(bufferevent *bev, void *arg)
{
	reinterpret_cast<IO_Thread*>(arg)->read_Callback(bev, arg);
}

static void write_Callback_Wrapper(bufferevent *bev, void *arg)
{
	reinterpret_cast<IO_Thread*>(arg)->write_Callback(bev, arg);
}

static void error_Callback_Wrapper(bufferevent *bev, short event, void *arg)
{
	reinterpret_cast<IO_Thread*>(arg)->error_Callback(bev, event, arg);
}

#endif // ! 



