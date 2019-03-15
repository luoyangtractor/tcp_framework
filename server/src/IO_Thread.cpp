#include "IO_Thread.h"


IO_Thread::IO_Thread(int max_con_num, int time_out):Thread_Base(),\
												m_max_con_num(max_con_num),\
												m_time_out(time_out),\
												p_thread(std::make_shared<std::thread>(&IO_Thread::run, this))
{
	m_thread_id = p_thread.get()->get_id();
	p_thread.get()->detach();
}


IO_Thread::~IO_Thread()
{
	this->stop();
}


bool IO_Thread::do_Work()
{
	return true;
}


void IO_Thread::run()
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


bool IO_Thread::add_Con(int sock_fd)
{
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


void IO_Thread::handle_Read(int sock_fd)
{

}


void IO_Thread::handle_Write(int sock_fd)
{

}


std::thread::id IO_Thread::get_Thread_Id()
{
	return m_thread_id;
}