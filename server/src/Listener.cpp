
#include "Listener.h"
#include "net_utils.h"


Listener::Listener(int port):m_port(port), m_listen_flag(false)
{

	p_event_base = event_base_new();
	bind_And_Listen(m_sock_fd);
	p_worker_thread_manager = std::make_shared<Worker_Thread_Manager>();
	p_io_thread_manager = std::make_shared<IO_Thread_Manager>(p_worker_thread_manager);

	p_listen_thread = std::make_shared<std::thread>(&Listener::run, this);
}


Listener::~Listener()
{
	this->stop();
}


bool Listener::bind_And_Listen(int sock_fd)
{
	int _ret = 0;
	//bind()
	//listen()
	_ret = evutil_make_socket_nonblocking(sock_fd);
	if (_ret < 0)
	{
		std::cout << "make listen sock non-blocking failed" << std::endl;
	}
	struct event *listen_event;
	listen_event = event_new(p_event_base, (evutil_socket_t)sock_fd, EV_READ | EV_PERSIST, accept_Callback_Wrapper, (void*)(this));
	event_add(listen_event, NULL);
	return true;
}


void Listener::accept_Callback(evutil_socket_t sock_fd, short event, void *arg)
{
	int _ret = 0;
	int con_fd = 0;//=accept();
	_ret = evutil_make_socket_nonblocking(con_fd);
	if (_ret < 0)
	{
		std::cout << "make accept sock non-blocking failed" << std::endl;
	}
	p_io_thread_manager->insert_New_Con(con_fd);
}


bool Listener::start()
{
	m_listen_flag = true;
	p_io_thread_manager->start();
	p_worker_thread_manager->start();
	return true;
}


bool Listener::run()
{
	event_base_dispatch(p_event_base);
	return true;
}



// for temperorily listener.stop()==listener.terminate()
bool Listener::stop()
{
	m_listen_flag = false;
	p_io_thread_manager->terminate();
	p_worker_thread_manager->terminate();
	return true;
}