#pragma once

#ifndef THREAD_BASE_H
#define THREAD_BASE_H

#include <thread>
#include <atomic>
#include <exception>
#include <iostream>

class Thread_Base
{
public:
	explicit Thread_Base();
	virtual ~Thread_Base();
	
	virtual bool stop();
	virtual void run();
	virtual void exception_Handle(std::exception& e);
	virtual bool do_Work();

	std::atomic<bool> m_running_flag;
};

#endif