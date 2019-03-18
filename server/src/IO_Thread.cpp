#include "IO_Thread.h"



IO_Thread::IO_Thread(std::shared_ptr<Worker_Thread_Manager> worker_thread_manager, int max_con_num, int time_out):Thread_Base(),\
												m_max_con_num(max_con_num),\
												m_time_out(time_out),\
												p_thread(std::make_shared<std::thread>(&IO_Thread::run, this))
{
	p_worker_thread_manager = worker_thread_manager;
	p_event_base = event_base_new();
	m_thread_id = p_thread->get_id();
	p_thread->join();
}


IO_Thread::~IO_Thread()
{
	this->stop();
	this->terminate();
}


bool IO_Thread::do_Work()
{
	event_base_dispatch(p_event_base);
	return true;
}


void IO_Thread::run()
{
	while (m_running_flag)
	{
		if (m_started_flag)
		{
			try {
				do_Work();
			}
			catch (std::exception e)
			{
				exception_Handle(e);
				this->stop();
				this->terminate();
			}
		}
	}
}


bool IO_Thread::add_Con(int sock_fd)
{
	bufferevent* _buffer_event = bufferevent_socket_new(p_event_base, sock_fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(_buffer_event, read_Callback_Wrapper, write_Callback_Wrapper, error_Callback_Wrapper, (void*)(this));
	bufferevent_enable(_buffer_event, EV_READ | EV_WRITE | EV_PERSIST);

	std::lock_guard<std::mutex> _locker(m_con_map_mutex);
	auto _con = std::make_shared<Tcp_Con>(sock_fd);
	m_con_map.insert(std::make_pair(sock_fd, _con));
	return true;
}


bool IO_Thread::remove_Con(int sock_fd)
{
	std::lock_guard<std::mutex> _locker(m_con_map_mutex);
	if (m_con_map.find(sock_fd) == m_con_map.end())
	{
		std::cout << "remove failed con did not exsit";
		return false;
	}
	else
	{
		for (auto iter = m_con_map.begin(); iter != m_con_map.end(); iter++)
		{
			if (iter->first == sock_fd) m_con_map.erase(iter);
			break;
		}
		return true;
	}
}


int IO_Thread::get_Con_Map_Size()
{
	std::lock_guard<std::mutex> _locker(m_con_map_mutex);
	return m_con_map.size();
}


void IO_Thread::read_Callback(bufferevent* buffer_event, void *arg)
{

}

void IO_Thread::write_Callback(bufferevent* buffer_event, void *arg)
{

}

void IO_Thread::error_Callback(bufferevent* buffer_event, short event, void *arg)
{

}


std::thread::id IO_Thread::get_Thread_Id()
{
	return m_thread_id;
}