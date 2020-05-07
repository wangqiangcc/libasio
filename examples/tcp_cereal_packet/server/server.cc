#include <iostream>
#include "simple_server.h"

int main()
{
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID, LOG_LEVEL_TRACE);
    ILog4zManager::getRef().setLoggerLimitsize(LOG4Z_MAIN_LOGGER_ID, 10);//10M
    ILog4zManager::getRef().setLoggerName(LOG4Z_MAIN_LOGGER_ID, "SimpleServer");
    ILog4zManager::getRef().setLoggerReserveDays(LOG4Z_MAIN_LOGGER_ID, 7);
    ILog4zManager::getRef().start();

    EventLoop loop;
    SimpleServer server(&loop, "0.0.0.0", 1188, "SimpleServer", 2);
    loop.Run();

	return 0;
}

