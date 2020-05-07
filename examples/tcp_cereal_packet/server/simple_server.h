#pragma once
#include "Packet.h"
#include "PacketCommon.h"
#include "tcp_server_impl.h"
#include "log4z.h"

using namespace zsummer::log4z;

class SimpleServer : public TcpServerImpl< PacketCommon::PacketHead >
{
public:
    SimpleServer(EventLoop* loop, const std::string &ip_address, uint16_t port, const std::string &name, uint32_t thread_num);
    ~SimpleServer();

public:
    void OnKeepAlive(const TCPConnPtr& conn, const std::shared_ptr<PacketCommon::KeepAlive> &pPkt);
    void OnLogin(const TCPConnPtr& conn, const std::shared_ptr<PacketCommon::Login> &pPkt);

private:
    virtual void OnConnect(const TCPConnPtr& conn);
    virtual void OnRecvData(const TCPConnPtr& conn, const std::shared_ptr<PacketCommon::PacketHead>& pPkt);
};

