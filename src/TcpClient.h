#ifndef TCP_CLIENT_H__
#define TCP_CLIENT_H__

#include <string>
#include <cstdint>
#include <memory>
#include <atomic>
#include <mutex>
#include <asio/steady_timer.hpp>
#include <asio/ip/tcp.hpp>
#include "TcpCallbacks.h"

using asio::steady_timer;
using asio::ip::tcp;

class EventLoop;
class TCPClient
{
public:
	TCPClient(EventLoop* loop, const std::string& remote_addr, uint16_t port, const std::string& name);
	~TCPClient();

	void Connect();
	void Disconnect();

	void SetConnCallback(const ConnCallback& cb);
	void SetMessageCallback(const MessageCallback& cb) { msg_fn_ = cb; }
	bool GetAutoReconnect() const { return auto_reconnect_;	}
	void SetAutoReconnect(bool v) { auto_reconnect_.store(v); }
	uint64_t GetReconnectInterval() const { return reconnect_interval_; }
	void SetReconnectInterval(uint64_t timeout) { reconnect_interval_ = timeout;}
		
	TCPConnPtr GetTCPConn() const;

	const std::string& RemoteAddress() const {	return remote_addr_; }
	uint16_t RemotePort() const { return remote_port_; }
	const std::string& GetName() const { return name_; }
	EventLoop* GetEventLoop() const { return loop_; }

private:
	void DisconnectInLoop();
	void HandleConn(tcp::socket&& socket);
	void OnRemoveConn(const TCPConnPtr& conn);
	void Reconnect();
private:
	EventLoop* loop_;

	tcp::socket socket_;

	std::string remote_addr_;
	uint16_t remote_port_;

	std::string name_;
	
	steady_timer reconnect_timer_;
	std::atomic<bool> auto_reconnect_; // The flag whether it reconnects automatically, Default : true
	uint64_t reconnect_interval_; // Default : 3 seconds
	int reconnecting_times_; // Default : 3 seconds

	mutable std::mutex mutex_; // The guard of conn_
	TCPConnPtr conn_;

	ConnCallback conn_fn_;
	MessageCallback msg_fn_;
};
#endif /// TCP_CLIENT_H__

