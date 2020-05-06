#ifndef EVENT_LOOP_THREAD_H__
#define EVENT_LOOP_THREAD_H__

#include <thread>
#include <functional>
#include <asio/ip/tcp.hpp>

using asio::ip::tcp;

class EventLoop;
class EventLoopThread
{
	typedef std::function<void()> Functor;
public:
	EventLoopThread();
	~EventLoopThread();

	bool Start(bool wait_thread_started = false, const Functor& pre = Functor(), const Functor& post = Functor());
	void Stop(bool wait_thread_exit = false);

	void SetName(const std::string& n);
	const std::string& name() const;

	EventLoop* GetEventLoop() const;

	bool IsRunning() const;
	bool IsStopped() const;

	tcp::socket* GetSocketForAccept() { return &accept_socket_; }
private:
	void Run(const Functor& pre, const Functor& post);

	std::shared_ptr<EventLoop> ev_loop_;
	std::shared_ptr<std::thread> thread_;
	tcp::socket	accept_socket_;

	enum Status {
		kRunning = 1,
		kStopping = 2,
		kStopped = 3,
	};
	Status status_;

	std::string name_;
};
#endif /// EVENT_LOOP_THREAD_H__

