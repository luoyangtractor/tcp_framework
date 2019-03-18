#include "Worker_Thread_Manager.h"



Worker_Thread_Manager::Worker_Thread_Manager(int thread_num, int max_wait_time, int max_queue_size):m_thread_number(thread_num),\
																									m_max_wait_time(max_wait_time),\
																									m_max_task_queue_size(max_queue_size)
{
	for (int i = 0; i < m_thread_number; i++)
	{
		auto _worker_thread = std::make_shared<Worker_Thread>(m_max_task_queue_size, m_max_wait_time);
		m_thread_pool.push_back(_worker_thread);
	}
}


bool Worker_Thread_Manager::stop()
{
	for (auto iter : m_thread_pool)
	{
		iter->stop();
	}
	return true;
}


bool Worker_Thread_Manager::start()
{
	for (auto iter : m_thread_pool)
	{
		iter->start();
	}
	return true;
}


bool Worker_Thread_Manager::terminate()
{
	for (auto iter : m_thread_pool)
	{
		iter->stop();
		iter->terminate();
	}
	return true;
}


Worker_Thread_Manager::~Worker_Thread_Manager()
{
	this->terminate();
}


std::future<Result> Worker_Thread_Manager::dispatch_Task(std::shared_ptr<Task> task)
{
	auto _suitable_thread  = choose_Suitable_Thread();
	std::cout<<"insert a task in thread "<< _suitable_thread->get_Thread_Id()<<std::endl;
	auto future = std::move(_suitable_thread->insert_Task(task));
	_suitable_thread->m_cv.notify_one();
	return future;
}



std::shared_ptr<Worker_Thread> Worker_Thread_Manager::choose_Suitable_Thread()
{
	std::shared_ptr<Worker_Thread>_min_load_thread = m_thread_pool[0];
	int _min_task_load = INT_MAX;
	for (auto iter : m_thread_pool)
	{
		if (iter->get_Task_Queue_Size() < _min_task_load)
		{
			_min_task_load = iter->get_Task_Queue_Size();
			_min_load_thread = iter;
		}
	}
	return _min_load_thread;
}