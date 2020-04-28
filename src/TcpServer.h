#ifndef TCP_SERVER_H__
#define TCP_SERVER_H__

#include <string>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <asio/ip/tcp.hpp>
#include "TcpCallbacks.h"

using asio::ip::tcp;
using asio::ip::address;

class EventLoop;
class EventLoopThreadPool;
class TCPServer
{
public:
	TCPServer(EventLoop* loop, const std::string& bind_ip, uint16_t port, const std::string& name, uint32_t thread_num);
	~TCPServer();

	bool Init();
	bool Start();
	void Stop();

	void SetConnectionCallback(const ConnCallback& cb) { conn_fn_ = cb;	}
	void SetMessageCallback(const MessageCallback& cb) { msg_fn_ = cb; }

	bool IsRunning() const;
	bool IsStopped() const;
	const std::string Address() const { 
		return acceptor_.local_endpoint().address().to_string(); 
	}

	void StopInLoop();
	void RemoveConn(const TCPConnPtr& conn);
	
protected:
	void AsyncAccept();
	void HandleNewConn(tcp::socket&& socket, EventLoop* loop);
private:
	EventLoop* loop_;  // the listening loop
	tcp::acceptor acceptor_;
	tcp::socket accept_socket_; // default accept this socket

	bool closed_;

	const std::string name_;
	
	std::shared_ptr<EventLoopThreadPool> tpool_;

	ConnCallback conn_fn_;
	MessageCallback msg_fn_;

	// always in the listening loop thread
	uint64_t next_conn_id_;
	typedef std::unordered_map<std::string, TCPConnPtr> ConnectionMap;
	ConnectionMap conns_;
};
#endif /// TCP_SERVER_H__

