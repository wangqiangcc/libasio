#include <iostream>
#include <string>
#include "event_loop.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "tcp_conn.h"
#include "serialize.h"

template<typename PACKET>
class TcpClientImpl
{
public:
    TcpClientImpl(EventLoop* loop, const std::string &remote_addr, uint16_t port, const std::string &name)
        : client_(new TCPClient(loop, remote_addr, port, name))
        , remote_addr_(remote_addr)
        , port_(port)
    {
        client_->Connect();
        client_->SetAutoReconnect(true);
        client_->SetConnCallback(std::bind(&TcpClientImpl::OnConnect_, this, std::placeholders::_1));
        client_->SetMessageCallback(std::bind(&TcpClientImpl::OnRecvData_, this, std::placeholders::_1, std::placeholders::_2));
    }

    ~TcpClientImpl(){}

public:
    void SendPkt(const TCPConnPtr& conn, std::shared_ptr<PACKET> pPkt);
    virtual void OnRecvData(const TCPConnPtr& conn, const std::shared_ptr<PACKET>& pPkt) = 0;
    virtual void OnConnect(const TCPConnPtr& conn) = 0;

    std::string GetRemoteAddress() const { return remote_addr_; }
    uint16_t GetPort() const { return port_; }

private:
    void OnConnect_(const TCPConnPtr& conn);
    void OnRecvData_(const TCPConnPtr& conn, ByteBuffer& buffer);

    std::string remote_addr_;
    uint16_t port_;
    std::shared_ptr<TCPClient> client_;
};

template<typename PACKET>
void TcpClientImpl<PACKET>::SendPkt(const TCPConnPtr& conn, std::shared_ptr<PACKET> pPkt)
{
    std::string ss = serialize::cereal_serialize(pPkt);
    conn->Send(ss);
}

template<typename PACKET>
void TcpClientImpl<PACKET>::OnConnect_(const TCPConnPtr &conn)
{
    OnConnect(conn);
}

template<typename PACKET>
void TcpClientImpl<PACKET>::OnRecvData_(const TCPConnPtr &conn, ByteBuffer &buffer)
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
