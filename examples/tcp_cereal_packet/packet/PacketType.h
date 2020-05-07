
#include <stdint.h>

namespace MessageType
{
	const uint16_t OPERATE_SUCCESS = 0;
	const uint16_t OPERATE_FAILED = -1;

    const uint32_t PACKET_KEEPALIVE = 0x00000001;
    const uint32_t PACKET_KEEPALIVE_ACK = 0x80000001;

    //Login
    const uint32_t PACKET_LOGIN = 0x00000002;
    const uint32_t PACKET_LOGIN_ACK = 0x80000002;
}
