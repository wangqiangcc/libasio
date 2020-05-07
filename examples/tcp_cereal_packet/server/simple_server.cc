#include <future>
#include "simple_server.h"

SimpleServer::SimpleServer(EventLoop* loop, const std::string &ip_address, uint16_t port, const std::string &name, uint32_t thread_num)
    :TcpServerImpl< PacketCommon::PacketHead>(loop, ip_address, port, name, thread_num)
{

}


SimpleServer::~SimpleServer()
{
}

void SimpleServer::OnKeepAlive(const TCPConnPtr& conn, const std::shared_ptr<PacketCommon::KeepAlive> &pPkt)
{
    std::shared_ptr<PacketCommon::KeepAliveAck> pKeepAlivePkt(new PacketCommon::KeepAliveAck);
    SendPkt(conn, pKeepAlivePkt);
}


void SimpleServer::OnLogin(const TCPConnPtr& conn, const std::shared_ptr<PacketCommon::Login> &pPkt)
{
    LOGFMTD("OnLogin name: %s, password : %s", pPkt->Name().c_str(), pPkt->PassWord().c_str());

    std::shared_ptr<PacketCommon::LoginAck> pLoginPkt(new PacketCommon::LoginAck);
    pLoginPkt->Ack(MessageType::OPERATE_SUCCESS);

    SendPkt(conn, pLoginPkt);
}

void SimpleServer::OnConnect(const TCPConnPtr &conn)
{
    if (conn->IsConnected()) {
        LOGFMTD("OnConnected: %s connection accepted.", conn->GetName().c_str());
    }
    else if (conn->IsDisconnecting()) {
        LOGFMTD("OnDisconnected: %s connection disconnecting.", conn->GetName().c_str());
    }
}

void SimpleServer::OnRecvData(const TCPConnPtr &conn, const std::shared_ptr<PacketCommon::PacketHead> &pPkt)
{
    if (!pPkt)
        return;

    LOGFMTD("OnRecvData type: 0x%08x", pPkt->Type());

    switch (pPkt->Type())
    {
    case MessageType::PACKET_KEEPALIVE:
        OnKeepAlive(conn, std::static_pointer_cast<PacketCommon::KeepAlive>(pPkt));
        break;
    case MessageType::PACKET_LOGIN:
        OnLogin(conn, std::static_pointer_cast<PacketCommon::Login>(pPkt));
        break;
    default:
        break;
    }
}
