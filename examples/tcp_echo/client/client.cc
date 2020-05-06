#include <iostream>
#include "event_loop.h"
#include "tcp_client.h"
#include "tcp_conn.h"

void OnConnection(const TCPConnPtr& conn) {
	if (conn->IsConnected()) {
		std::cout << conn->GetName() <<" connection accepted.\n";
                conn->Send("client say hello!");
	}
	else if (conn->IsDisconnecting()) {
		std::cout << conn->GetName() <<" connection disconnecting.\n";
	}
}

void OnMessage(const TCPConnPtr& conn, ByteBuffer& buffer) {
    std::cout << "recv msg " << std::string(buffer.Data(), buffer.Size()) << std::endl;
    buffer.ReadBytes(buffer.Size());
    buffer.Normalize();
    conn->Send("client say hello!");
}

int main()
{
	EventLoop loop;
    TCPClient client(&loop, "127.0.0.1", 7788, "EchoClient");
	client.Connect();
	client.SetAutoReconnect(true);
	client.SetConnCallback(OnConnection);
	client.SetMessageCallback(OnMessage);

	loop.Run();

	return 0;
}

