#include <iostream>
#include "EventLoop.h"
#include "TcpServer.h"
#include "TcpConn.h"

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
    conn->Send("server say hello!");
    //conn->Send(buffer.ReadBegin(), buffer.Size());
    //buffer.ReadBytes(buffer.Size());
    //conn->Close();
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

