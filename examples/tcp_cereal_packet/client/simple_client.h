#pragma once
#include "Packet.h"
#include "tcp_client_impl.h"
#include "PacketCommon.h"
#include "log4z.h"

using namespace zsummer::log4z;

class SimpleClient : public TcpClientImpl< PacketCommon::PacketHead >
{
public:
    SimpleClient(EventLoop* loop, const std::string &remote_addr, uint16_t port, const std::string &name);
	~SimpleClient();

public:


private:
    bool DoKeepAlve();
    void OnKeepAliveAck(const std::shared_ptr<PacketCommon::KeepAliveAck> &pPkt);
    void OnLoginAck(const std::shared_ptr<PacketCommon::LoginAck> &pPkt);

private:
    virtual void OnConnect(const TCPConnPtr& conn);
    virtual void OnRecvData(const TCPConnPtr& conn, const std::shared_ptr<PacketCommon::PacketHead>& pPkt);
    bool SendPkt(std::shared_ptr<PacketCommon::PacketHead> pPkt);

private:
    bool is_login_;
    TCPConnPtr conn_;
    steady_timer keepalive_timer_;
};
