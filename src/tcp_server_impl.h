#include <iostream>
#include <string>
#include "event_loop.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "tcp_conn.h"
#include "serialize.h"

template<typename PACKET>
class TcpServerImpl
{
public:
    TcpServerImpl(EventLoop* loop, const std::string &ip_address, uint16_t port, const std::string &name, uint32_t thread_num)
        : server_(new TCPServer(loop, ip_address, port, name, thread_num)) {
        server_->SetConnectionCallback(std::bind(&TcpServerImpl::OnConnect_, this, std::placeholders::_1));
        server_->SetMessageCallback(std::bind(&TcpServerImpl::OnRecvData_, this, std::placeholders::_1, std::placeholders::_2));
        server_->Init();
        server_->Start();
    }

    ~TcpServerImpl(){}

public:
    void SendPkt(const TCPConnPtr& conn, std::shared_ptr<PACKET> pPkt);
    virtual void OnRecvData(const TCPConnPtr& conn, const std::shared_ptr<PACKET>& pPkt) = 0;
    virtual void OnConnect(const TCPConnPtr& conn) = 0;

    const std::string Address() const { return server_->Address(); }

private:
    void OnConnect_(const TCPConnPtr& conn);
    void OnRecvData_(const TCPConnPtr& conn, ByteBuffer& buffer);

    std::shared_ptr<TCPServer> server_;
};

template<typename PACKET>
void TcpServerImpl<PACKET>::SendPkt(const TCPConnPtr& conn, std::shared_ptr<PACKET> pPkt)
{
    std::string ss = serialize::cereal_serialize(pPkt);
    conn->Send(ss);
}

template<typename PACKET>
void TcpServerImpl<PACKET>::OnConnect_(const TCPConnPtr &conn)
{
    OnConnect(conn);
}

template<typename PACKET>
void TcpServerImpl<PACKET>::OnRecvData_(const TCPConnPtr &conn, ByteBuffer &buffer)
{
    std::string ss;
    ss.assign(buffer.Data(), buffer.Size());

    buffer.ReadBytes(buffer.Size());
    buffer.Normalize();

    if (ss[0] != '\x1')
        return;

    std::shared_ptr<PACKET> pPkt = serialize::cereal_deserialize_ptr<PACKET>(ss);

    OnRecvData(conn, pPkt);
}
