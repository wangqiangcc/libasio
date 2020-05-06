#include <iostream>
#include "event_loop.h"
#include "tcp_server.h"
#include "tcp_conn.h"

void OnConnection(const TCPConnPtr& conn) {
	if (conn->IsConnected()) {
		std::cout << conn->GetName() <<" connection accepted.\n";
	}
	else if (conn->IsDisconnecting()) {
		std::cout << conn->GetName() <<" connection disconnecting.\n";
	}
}

void OnMessage(const TCPConnPtr& conn, ByteBuffer& buffer) {
    std::cout << "recv msg " << std::string(buffer.Data(), buffer.Size()) << std::endl;
    buffer.ReadBytes(buffer.Size());
    buffer.Normalize();

    conn->Send("server say hello!");
}

int main()
{
	EventLoop loop;
	TCPServer s(&loop, "0.0.0.0", 7788, "EchoServer", 4);
	s.SetConnectionCallback(OnConnection);
	s.SetMessageCallback(OnMessage);
	s.Init();
	s.Start();

	loop.Run();

	return 0;
}

