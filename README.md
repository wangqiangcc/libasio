# libasio

A modern C++ network library based on asio for high performance network services

sample tcp server

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
    buffer.ReadBytes(buffer.Size());
    buffer.Normalize();

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


sample tcp client

#include <iostream>
#include "EventLoop.h"
#include "TcpClient.h"
#include "TcpConn.h"

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
    conn->Send("client say hello!");
    buffer.ReadBytes(buffer.Size());
    buffer.Normalize();
}

int main()
{
	EventLoop loop;
	TCPClient client(&loop, "127.0.0.1", 7788, "MYDOOP");
	client.Connect();
	client.SetAutoReconnect(true);
	client.SetConnCallback(OnConnection);
	client.SetMessageCallback(OnMessage);

	loop.Run();

	return 0;
}
