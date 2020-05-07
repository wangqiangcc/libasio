#pragma once
#include <string>
#include "Packet.h"
#include "PacketType.h"

namespace PacketCommon
{


struct KeepAlive : public PacketHead
{
public:
    KeepAlive() : PacketHead(MessageType::PACKET_KEEPALIVE){}

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive &archive) {
        archive(cereal::base_class<PacketHead>(this));
    }
};

struct KeepAliveAck : public PacketHead
{
public:

    KeepAliveAck() : PacketHead(MessageType::PACKET_KEEPALIVE_ACK){}

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive &archive) {
        archive(cereal::base_class<PacketHead>(this));
    }
};


struct Login : public PacketHead
{
public:

    Login() : PacketHead(MessageType::PACKET_LOGIN) { }

    Login(uint64_t id, std::string name, std::string password) :
        PacketHead(MessageType::PACKET_LOGIN),
        id_(id),
        name_(name),
        password_(password){}

    uint64_t Id() const { return id_; }
    void Id(uint64_t id) { id_ = id; }

    std::string& Name() { return name_; }
    const std::string& Name() const { return name_; }

    std::string& PassWord() { return password_; }
    const std::string& PassWord() const { return password_; }

private:
    friend class cereal::access;
    uint64_t id_;
    std::string name_;
    std::string password_;
    template<class Archive>
    void serialize(Archive &archive) {
        archive(cereal::base_class<PacketHead>(this), id_, name_, password_);
    }
};

struct LoginAck : public PacketHead
{
public:

    LoginAck() : PacketHead(MessageType::PACKET_LOGIN_ACK) { }

    LoginAck(uint64_t ack) :
        PacketHead(MessageType::PACKET_LOGIN_ACK),
        ack_(ack) {}

    uint64_t Ack() const { return ack_; }
    void Ack(uint64_t ack) { ack_ = ack; }

private:
    friend class cereal::access;
    uint64_t ack_;
    template<class Archive>
    void serialize(Archive &archive) {
        archive(cereal::base_class<PacketHead>(this), ack_);
    }
};

}

CEREAL_REGISTER_TYPE(PacketCommon::KeepAlive)
CEREAL_REGISTER_TYPE(PacketCommon::KeepAliveAck)
CEREAL_REGISTER_TYPE(PacketCommon::Login)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(PacketCommon::PacketHead, PacketCommon::Login);
CEREAL_REGISTER_TYPE(PacketCommon::LoginAck)
