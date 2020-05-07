#include "simple_client.h"

SimpleClient::SimpleClient(EventLoop* loop, const std::string &remote_addr, uint16_t port, const std::string &name)
    : TcpClientImpl< PacketCommon::PacketHead >(loop, remote_addr, port, name)
    , is_login_(false)
    , keepalive_timer_(loop->io_service())
{
}


SimpleClient::~SimpleClient()
{

}

bool SimpleClient::DoKeepAlve()
{
    if (!is_login_) {
        return false;
    }

    keepalive_timer_.expires_at(keepalive_timer_.expires_at() + std::chrono::seconds(3));
    keepalive_timer_.async_wait(std::bind(&SimpleClient::DoKeepAlve, this));

    std::shared_ptr<PacketCommon::KeepAlive> pPkt(new PacketCommon::KeepAlive);
    return SendPkt(pPkt);
}

void SimpleClient::OnKeepAliveAck(const std::shared_ptr<PacketCommon::KeepAliveAck> &pPkt)
{
    LOGFMTT("OnKeepAliveAck");
}

void SimpleClient::OnLoginAck(const std::shared_ptr<PacketCommon::LoginAck> &pPkt)
{
    LOGFMTI("OnLoginAck Id: %llu", pPkt->Ack());

    if(pPkt->Ack() == 0) {
        LOGI("Login success!");
        is_login_ = true;
        // keeepalive
        keepalive_timer_.expires_from_now(std::chrono::seconds(3));
        keepalive_timer_.async_wait(std::bind(&SimpleClient::DoKeepAlve, this));

    } else {
        is_login_ = false;
        LOGFMTI("Login failed Ack: %llu", pPkt->Ack());
    }

}

void SimpleClient::OnConnect(const TCPConnPtr &conn)
{
    if (conn->IsConnected()) {
        LOGFMTD("OnConnected: %s connection accepted.", conn->GetName().c_str());
        conn_ = conn;
        std::shared_ptr<PacketCommon::Login> pPkt(new PacketCommon::Login);
        pPkt->Name() = "test";
        pPkt->PassWord() = "abc#123";
        SendPkt(pPkt);
    }
    else if (conn->IsDisconnecting()) {
        LOGFMTD("OnDisconnected: %s connection disconnecting.", conn->GetName().c_str());
        is_login_ = false;
    }
}

void SimpleClient::OnRecvData(const TCPConnPtr &conn, const std::shared_ptr<PacketCommon::PacketHead> &pPkt)
{
    LOGFMTI("OnRecvData type: 0x%08x", pPkt->Type());
    if (!pPkt) return;
    switch (pPkt->Type()) {
    case MessageType::PACKET_KEEPALIVE_ACK:
        OnKeepAliveAck(std::static_pointer_cast<PacketCommon::KeepAliveAck>(pPkt));
        break;
    case MessageType::PACKET_LOGIN_ACK:
        OnLoginAck(std::static_pointer_cast<PacketCommon::LoginAck>(pPkt));
        break;
    default:
        break;
    }
}


bool SimpleClient::SendPkt(std::shared_ptr<PacketCommon::PacketHead> pPkt)
{
    if (!conn_) {
        LOGFMTE("SendPkt Failed, %s is INVALID PktType=0x%08x.", conn_->GetName().c_str(), pPkt->Type());
        return false;
    }
    TcpClientImpl < PacketCommon::PacketHead >::SendPkt(conn_, pPkt);

    return true;
}
