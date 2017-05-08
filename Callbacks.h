#ifndef CALLBACK_H__
#define CALLBACK_H__

#include <memory>
#include <functional>
#include "ByteBuffer.h"

class TCPConn;

typedef std::shared_ptr<TCPConn> TCPConnPtr;
typedef std::function<void ()> TimerCallback;

// When a connection established, broken down, connecting failed, this callback will be called
// This is called from a work-thread this is not the listening thread probably
typedef std::function<void (const TCPConnPtr&)> ConnCallback;

typedef std::function<void (const TCPConnPtr&)> CloseCallback;
typedef std::function<void (const TCPConnPtr&)> WriteCompleteCallback;

typedef std::function<void (const TCPConnPtr&, size_t)> HighWaterMarkCallback;

typedef std::function<void (const TCPConnPtr&, ByteBuffer&)> MessageCallback;



inline void DefaultConnectionCallback(const TCPConnPtr&) {}
inline void DefaultMessageCallback(const TCPConnPtr&, ByteBuffer&) {}


#endif
