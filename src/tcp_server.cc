#include "event_loop.h"
#include "event_loop_thread_pool.h"
#include "tcp_server.h"
#include "tcp_conn.h"

TCPServer::TCPServer(EventLoop* loop, const std::string& bind_ip, uint16_t port, const std::string& name, uint32_t thread_num)
	: loop_(loop)
	, acceptor_(loop_->io_service(), tcp::endpoint(address::from_string(bind_ip), port))
	, accept_socket_(loop_->io_service())
	, closed_(false)
	, name_(name)
	, tpool_(nullptr)
	, conn_fn_(nullptr)
	, msg_fn_(nullptr)
	, next_conn_id_(0)
{
	tpool_.reset(new EventLoopThreadPool(loop_, thread_num));
}

TCPServer::~TCPServer()
{
	assert(tpool_->IsStopped());
	assert(conns_.empty());
	assert(!acceptor_.is_open());
	tpool_.reset();
}

bool TCPServer::Init()
{
	
	return true;
}

bool TCPServer::Start()
{
	if (!tpool_->Start(true)) {
		return false;
	}
	AsyncAccept();
	return true;
}

void TCPServer::Stop()
{
	loop_->RunInLoop(std::bind(&TCPServer::StopInLoop, this));
}

void TCPServer::StopInLoop() {
    asio::error_code ec;
	acceptor_.close(ec);

	for (auto& c : conns_) {
		c.second->Close();
	}

	tpool_->Stop(true);
	assert(tpool_->IsStopped());
	closed_ = true;
}

bool TCPServer::IsRunning() const {
	if (!loop_->IsRunning()) {
		return false;
	}

	if (!tpool_->IsRunning()) {
		return false;
	}

	assert(loop_->IsRunning() && tpool_->IsRunning());
	return true;
}

bool TCPServer::IsStopped() const {
	if (!loop_->IsStopped()) {
		return false;
	}

	if (!tpool_->IsStopped()) {
		return false;
	}

	assert(loop_->IsStopped() && tpool_->IsStopped());
	return true;
}

void TCPServer::RemoveConn(const TCPConnPtr& conn)
{
	auto f = [=]() {
		// Remove the connection in the listening EventLoop
		assert(this->loop_->IsInLoopThread());
		this->conns_.erase(conn->GetName());
	};
	loop_->RunInLoop(f);
}

void TCPServer::AsyncAccept()
{
	tcp::socket* socket = nullptr;
	EventLoop* loop = nullptr;
	if (tpool_->GetThreadNum() > 0) {
		std::tie(socket, loop) = tpool_->GetSocketForAccept();
	}
	else {
		// default
		socket = &accept_socket_;
		loop = loop_;
	}
	
    acceptor_.async_accept(*socket, [this, socket, loop](asio::error_code ec)
	{
		if (!ec) {
            asio::error_code error;
			socket->non_blocking(true, error);
			this->HandleNewConn(std::move(*socket), loop);
		}
		else {
			// error notice
		}
		if (!closed_) {
			this->AsyncAccept();
		}
	});
}

void TCPServer::HandleNewConn(tcp::socket&& socket, EventLoop* loop)
{
	assert(loop_->IsInLoopThread());
	std::string cn = name_ + "-conn" + "#" + std::to_string(next_conn_id_++); // TODO use string buffer
	TCPConnPtr conn = std::make_shared<TCPConn>(loop, std::move(socket), cn);
	assert(conn->GetType() == TCPConn::kIncoming);
	conn->SetMessageCallback(msg_fn_);
	conn->SetConnCallback(conn_fn_);
	conn->SetCloseCallback(std::bind(&TCPServer::RemoveConn, this, std::placeholders::_1));
	loop->RunInLoop(std::bind(&TCPConn::OnAttachedToLoop, conn));
	conns_[cn] = conn;
}
