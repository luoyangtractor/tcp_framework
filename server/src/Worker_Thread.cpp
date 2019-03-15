#include "Worker_Thread.h"
#include "thread_global_def.h"


Worker_Thread::Worker_Thread(int max_queue_size, int max_wait_time):Thread_Base(),\
															m_max_queue_size(max_queue_size),\
															m_empty_flag(true),\
															m_thread_status(READY),\
															m_max_wait_time(max_wait_time),\
															p_thread(std::make_shared<std::thread>(&Worker_Thread::run, this)),\
															p_helper(std::make_shared<Helper>())
{
	m_thread_id = p_thread.get()->get_id();
	p_thread.get()->detach();
}


Worker_Thread::~Worker_Thread()
{
	this->stop();
}


void Worker_Thread::run()
{
	while (m_running_flag)
	{
		try {
			do_Work();
		}
		catch (std::exception e)
		{
			exception_Handle(e);
			stop();
		}
	}
}


bool Worker_Thread::do_Work()
{
	while(m_empty_flag)
	{
		m_thread_status = READY;
		std::unique_lock<std::mutex> _cv_lock(m_cv_mutex);
		m_cv.wait(_cv_lock);
	}

	m_thread_status = RUNNING;

	m_queue_mutex.lock();
	auto _work = std::move(m_task_queue.front().first);
	time_point _insert_time = std::move(m_task_queue.front().second);
	m_task_queue.pop();
	if (m_task_queue.empty())m_empty_flag = true;
	m_queue_mutex.unlock();

	// process task

	time_point _proc_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());

	if (_proc_time.time_since_epoch().count() - _insert_time.time_since_epoch().count() <= m_max_wait_time)
	{
		//std::cout<< "wait_time: "<< _tp.time_since_epoch().count() - _insert_time.time_since_epoch().count() << std::endl;
		//std::cout << std::this_thread::get_id() <<" task_queue size: " << _task_queue.size() << std::endl;

		_work(p_helper.get());
	}
	else
	{
		std::cout << "task timeout" << std::endl;
	}
	return true;
}


std::future<Result> Worker_Thread::insert_Task(std::function<Result(Helper*)> work)
{
	std::packaged_task<Result(Helper*)> _task(work);
	std::future<Result> _future_result = _task.get_future();
	m_queue_mutex.lock();
	//std::cout << "insert a tast  task_queue size: " << _task_queue.size()<< std::endl;

	if (m_max_queue_size > 0)
	{
		if (m_task_queue.size() < m_max_queue_size)
		{
			time_point _insert_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
			m_task_queue.push(std::move(std::make_pair(std::move(_task), _insert_time)));
		}
		else
		{
			while (m_task_queue.size() >= m_max_queue_size)
			{
				m_task_queue.pop();
			}
			time_point _insert_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
			m_task_queue.push(std::move(std::make_pair(std::move(_task), _insert_time)));
		}
	}
	else
	{
		time_point _insert_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
		m_task_queue.push(std::move(std::make_pair(std::move(_task), _insert_time)));
	}
	m_queue_mutex.unlock();
	m_empty_flag = false;
	return _future_result;
}


int Worker_Thread::get_Task_Queue_Size()
{
	std::unique_lock<std::mutex> locker(m_queue_mutex);
	int _size = m_task_queue.size();
	return _size;
}


std::thread::id Worker_Thread::get_Thread_Id()
{
	return m_thread_id;
}
