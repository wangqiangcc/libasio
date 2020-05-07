#include <iostream>
#include "simple_client.h"

int main()
{
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID, LOG_LEVEL_TRACE);
    ILog4zManager::getRef().setLoggerLimitsize(LOG4Z_MAIN_LOGGER_ID, 10);//10M
    ILog4zManager::getRef().setLoggerName(LOG4Z_MAIN_LOGGER_ID, "NetworkClient");
    ILog4zManager::getRef().setLoggerReserveDays(LOG4Z_MAIN_LOGGER_ID, 7);
    ILog4zManager::getRef().start();

    EventLoop loop;
    SimpleClient client(&loop, "127.0.0.1", 1188, "SimpleClient");
    loop.Run();

	return 0;
}

