#ifndef EVENT_LOOP_THREAD_POOL_H__
#define EVENT_LOOP_THREAD_POOL_H__

#include <atomic>
#include <cstdint>
#include <memory>
#include <vector>
#include <asio/ip/tcp.hpp>

using asio::ip::tcp;

class EventLoop;
class EventLoopThread;
class EventLoopThreadPool 
{
public:
	EventLoopThreadPool(EventLoop* base_loop, uint32_t thread_num);
	~EventLoopThreadPool();

	bool Start(bool wait_thread_started = false);
	void Stop(bool wait_thread_exit = false);

	EventLoop* GetNextLoop();

	std::pair<tcp::socket*, EventLoop*> GetSocketForAccept();

	bool IsRunning() const;
	bool IsStopped() const;

	uint32_t GetThreadNum() const;

private:
	EventLoop* ev_loop_;
	bool started_;
	uint32_t thread_num_;
	std::atomic<int64_t> next_;

	typedef std::shared_ptr<EventLoopThread> EventLoopThreadPtr;
	std::vector<EventLoopThreadPtr> threads_;
};

#endif /// EVENT_LOOP_THREAD_POOL_H__
