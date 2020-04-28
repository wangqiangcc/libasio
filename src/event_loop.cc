#include "event_loop.h"

EventLoop::EventLoop()
	: io_service_()
	, work_(io_service_)
	, running_(false)
	, tid_(std::this_thread::get_id()) // default thread id
{

}

EventLoop::~EventLoop()
{

}

void EventLoop::Run()
{
	tid_ = std::this_thread::get_id();
	running_ = true;
	
    asio::error_code ec;
	io_service_.run(ec);
	
	running_ = false;
}


void EventLoop::Stop()
{
	assert(running_);
	RunInLoop(std::bind(&EventLoop::StopInLoop, this));
}

void EventLoop::StopInLoop()
{
	io_service_.stopped();
}

void EventLoop::RunInLoop(const Functor& functor)
{
	if (IsInLoopThread()) {
		functor();
	}
	else {
		QueueInLoop(functor);
	}
}

void EventLoop::RunInLoop(Functor&& functor)
{
	if (IsInLoopThread()) {
		functor();
	}
	else {
		QueueInLoop(std::move(functor));
	}
}

void EventLoop::QueueInLoop(const Functor& functor)
{
	io_service_.post(functor);
}

void EventLoop::QueueInLoop(Functor&& functor)
{
	io_service_.post(functor);
}
