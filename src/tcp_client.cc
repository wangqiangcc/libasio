#include <asio/connect.hpp>
#include "event_loop.h"
#include "tcp_client.h"
#include "tcp_conn.h"

static const int default_max_reconnect_time = 5;

TCPClient::TCPClient(EventLoop* loop, const std::string& remote_addr, uint16_t port, const std::string& name)
	: loop_(loop)
	, socket_(loop_->io_service())
    , remote_addr_(remote_addr)
	, remote_port_(port)
	, name_(name)
	, reconnect_timer_(loop_->io_service())
	, auto_reconnect_(false)
	, reconnect_interval_(3)
	, reconnecting_times_(0)
	, mutex_()
	, conn_()
	, conn_fn_(nullptr)
	, msg_fn_(nullptr)
{

}

TCPClient::~TCPClient()
{
	auto_reconnect_.store(false);
	TCPConnPtr c = GetTCPConn();
	if (c) {
		// Most of the cases, the conn_ is at disconnected status at this time.
		// But some times, the user application layer will call TCPClient::Close()
		// and delete TCPClient object immediately, that will make conn_ to be at disconnecting status.
		assert(c->IsDisconnected() || c->IsDisconnecting());
		if (c->IsDisconnecting()) {
			// the reference count includes :
			//  - this
			//  - c
			//  - A disconnecting callback which hold a shared_ptr of TCPConn
			assert(c.use_count() >= 3);
			c->SetCloseCallback(CloseCallback());
		}
	}
	conn_.reset();
}


void TCPClient::Connect()
{
	tcp::resolver resolver(loop_->io_service());
	auto endpoint_iterator = resolver.resolve({remote_addr_, std::to_string(remote_port_)});
    asio::async_connect(socket_, endpoint_iterator,
        [this](asio::error_code ec, tcp::resolver::iterator)
	{
		if (!ec) {
			reconnecting_times_ = 0;
			this->HandleConn(std::move(socket_));
		}
		else {
			if (auto_reconnect_ && reconnecting_times_ < default_max_reconnect_time) {
                reconnect_timer_.expires_from_now(std::chrono::seconds(reconnect_interval_));
				reconnect_timer_.async_wait(std::bind(&TCPClient::Reconnect, this));
			} 
			else {
				/// 
			}
		}
	});
}

void TCPClient::Reconnect() {
	++reconnecting_times_;
	Connect();
}

void TCPClient::Disconnect() 
{
	loop_->RunInLoop(std::bind(&TCPClient::DisconnectInLoop, this));
}

void TCPClient::DisconnectInLoop() {
	assert(loop_->IsInLoopThread());
	auto_reconnect_.store(false);

	if (conn_) {
		assert(!conn_->IsDisconnected() && !conn_->IsDisconnecting());
		conn_->Close();
	}
}


void TCPClient::SetConnCallback(const ConnCallback& cb)
{
	conn_fn_ = cb;
	if (conn_) {
		conn_->SetConnCallback(conn_fn_);
	}
}

void TCPClient::HandleConn(tcp::socket&& socket) {
	if (!socket.is_open()) {
		conn_fn_(TCPConnPtr(new TCPConn(loop_, std::move(socket), name_)));
		return;
	}

    assert(loop_->IsInLoopThread());
	TCPConnPtr c = TCPConnPtr(new TCPConn(loop_, std::move(socket), name_));
	c->SetType(TCPConn::kOutgoing);
	c->SetMessageCallback(msg_fn_);
	c->SetConnCallback(conn_fn_);
	c->SetCloseCallback(std::bind(&TCPClient::OnRemoveConn, this, std::placeholders::_1));
	
	{
		std::lock_guard<std::mutex> lock(mutex_);
		conn_ = c;
	}

	c->OnAttachedToLoop();
}


void TCPClient::OnRemoveConn(const TCPConnPtr& c) {
    if(!conn_)
        return;
    assert(c.get() == conn_.get());
	assert(loop_->IsInLoopThread());
	conn_.reset();
	if (auto_reconnect_.load()) {
		Reconnect();
	}
}


TCPConnPtr TCPClient::GetTCPConn() const
{
	if (loop_->IsInLoopThread()) {
		return conn_;
	}
	else {
		// If it is not in the loop thread, we should add a lock here
		std::lock_guard<std::mutex> guard(mutex_);
		TCPConnPtr c = conn_;
		return c;
	}
}
