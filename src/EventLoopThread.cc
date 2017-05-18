#include <sstream>
#include "EventLoopThread.h"
#include "EventLoop.h"

EventLoopThread::EventLoopThread()
	: ev_loop_(new EventLoop)
	, thread_(nullptr)
	, accept_socket_(ev_loop_->io_service())
	, status_(kStopped)
{

}

EventLoopThread::~EventLoopThread()
{
	if (thread_ && thread_->joinable()) {
		try {
			thread_->join();
		}
		catch (const std::system_error& ) {
			
		}
	}
}

bool EventLoopThread::Start(bool wait_thread_started, const Functor& pre, const Functor& post)
{
	thread_.reset(new std::thread(std::bind(&EventLoopThread::Run, this, pre, post)));
	while (wait_thread_started) {
		if (IsRunning()) {
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds() * 100);
	}

	return true;
}

void EventLoopThread::Run(const Functor& pre, const Functor& post)
{
	if (name_.empty()) {
		std::ostringstream os;
		os << "thread-" << std::this_thread::get_id();
		name_ = os.str();
	}
	status_ = kRunning;
	if (pre) {
		pre();
	}
	ev_loop_->Run();

	if (post) {
		post();
	}
	ev_loop_.reset(); // Make sure construct, initialize and destruct in the same thread
	status_ = kStopped;
}


void EventLoopThread::Stop(bool wait_thread_exit)
{
	assert(status_ == kRunning && IsRunning());
	status_ = kStopping;
	ev_loop_->Stop();

	if (wait_thread_exit) {
		while (!IsStopped()) {
			std::this_thread::sleep_for(std::chrono::milliseconds() * 100);
		}
		if (thread_->joinable()) {
			try {
				thread_->join();
			}
			catch (const std::system_error& ) {
				
			}
		}
	}
}

void EventLoopThread::SetName(const std::string& n)
{
	name_ = n;
}

const std::string& EventLoopThread::name() const
{
	return name_;
}

EventLoop* EventLoopThread::GetEventLoop() const
{
	return ev_loop_.get();
}

bool EventLoopThread::IsRunning() const
{
	return ev_loop_->IsRunning();
}

bool EventLoopThread::IsStopped() const
{
	return status_ == kStopped;
}



