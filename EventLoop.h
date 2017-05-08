#ifndef EVENT_LOOP_H__
#define EVENT_LOOP_H__

#include <thread>
#include <functional>
#include <atomic>
#include <cassert>
#include <asio/io_service.hpp>

class EventLoop
{
	typedef std::function<void()> Functor;
public:
	EventLoop();
	~EventLoop();

	void Run();
	void Stop();

	void RunInLoop(const Functor& functor);
	void RunInLoop(Functor&& functor);

	void QueueInLoop(const Functor& functor);
	void QueueInLoop(Functor&& functor);

	bool IsInLoopThread() const {
		return tid_ == std::this_thread::get_id();
	}

	void AssertInLoopThread() const {
		assert(IsInLoopThread());
	}

	bool IsRunning() const { return running_; }
	bool IsStopped() const { return !IsRunning(); }

    asio::io_service& io_service() { return io_service_; }

private:
	

	void StopInLoop();
private:
    asio::io_service io_service_;
    asio::io_service::work work_;

	std::atomic<bool> running_;
	std::thread::id tid_;


};
#endif /// EVENT_LOOP_H__

