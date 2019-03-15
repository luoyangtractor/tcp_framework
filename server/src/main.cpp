#include "Worker_Thread_Manager.h"
#include <iostream>


Result test_work(Helper* m_helper)
{
	int tmp = 0;
	for (int i = 0;i<10000000;i++)
	{
		tmp += 1;
	}
	Result r;
	return r;
}


void worker_Thread_Test()
{
	Worker_Thread_Manager* manager = new Worker_Thread_Manager(8, 500);
	Helper _helper;
	time_point start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
	for (int i = 0; i < 1000; i++)
	{
		test_work(&_helper);
	}
	time_point end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
	std::cout << std::endl << std::endl << end.time_since_epoch().count() - start.time_since_epoch().count() << " ms" << std::endl;


	start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
	for (int i = 0; i < 1000; i++)
	{
		auto future = std::move(manager->dispatch_Work(std::bind(test_work, std::placeholders::_1)));
		//future.wait();
	}
	end = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
	std::cout << std::endl << std::endl << end.time_since_epoch().count() - start.time_since_epoch().count() << " ms" << std::endl;
}


int main()
{
	worker_Thread_Test();
	int i;
	std::cin >> i;
	return 0;
}
