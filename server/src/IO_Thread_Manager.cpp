#include "IO_Thread_Manager.h"



IO_Thread_Manager::IO_Thread_Manager(std::shared_ptr<Worker_Thread_Manager> worker_thread_manager, int thread_num , int max_wait_time, int max_con_map_size):
																		m_max_con_map_size(max_con_map_size),\
																		m_max_wait_time(max_wait_time),\
																		m_thread_number(thread_num)
{
	p_worker_thread_manager = worker_thread_manager;
	for (int i = 0; i < m_thread_number; i++)
	{
		auto _worker_thread = std::make_shared<IO_Thread>(p_worker_thread_manager, m_max_con_map_size, m_max_wait_time);
		m_thread_pool.push_back(_worker_thread);
	}
}


bool IO_Thread_Manager::stop()
{
	for (auto iter : m_thread_pool)
	{
		iter->stop();
	}
	return true;
}

bool IO_Thread_Manager::start()
{
	for (auto iter : m_thread_pool)
	{
		iter->start();
	}
	return true;
}

bool IO_Thread_Manager::terminate()
{
	for (auto iter : m_thread_pool)
	{
		iter->stop();
		iter->terminate();
	}
	return true;
}


IO_Thread_Manager::~IO_Thread_Manager()
{
	this->terminate();
}


bool IO_Thread_Manager::insert_New_Con(int sock_fd)
{
	auto _suitable_thread = choose_Suitable_Thread();
	std::cout << "insert a tcp_con in thread " << _suitable_thread->get_Thread_Id() << std::endl;
	_suitable_thread->add_Con(sock_fd);
	return true;
}



std::shared_ptr<IO_Thread> IO_Thread_Manager::choose_Suitable_Thread()
{
	std::shared_ptr<IO_Thread>_min_load_thread = m_thread_pool[0];
	int _min_task_load = INT_MAX;
	for (auto iter : m_thread_pool)
	{
		if (iter->get_Con_Map_Size() < _min_task_load)
		{
			_min_task_load = iter->get_Con_Map_Size();
			_min_load_thread = iter;
		}
	}
	return _min_load_thread;
}
