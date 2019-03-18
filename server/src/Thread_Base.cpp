#include "Thread_Base.h"



Thread_Base::Thread_Base():m_running_flag(true), m_started_flag(false)
{
	
}


Thread_Base::~Thread_Base()
{
	
}


bool Thread_Base::start()
{
	m_started_flag = true;
	return true;
}


bool Thread_Base::stop()
{
	m_started_flag = false;
	return true;
}


bool Thread_Base::terminate()
{
	m_running_flag = false;
	return true;
}


void Thread_Base::run()
{
	while (m_running_flag)
	{

	}
}

bool Thread_Base::do_Work()
{
	return true;
}


void Thread_Base::exception_Handle(std::exception& e)
{
	std::cout << "thead:" <<std::this_thread::get_id()<<" got exception: " << e.what()<<" stopped";
}
