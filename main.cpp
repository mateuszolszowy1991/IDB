#include <iostream>
#include <memory>
#include <string>
#include <cstdint>
#include "MainContainer.hpp"
#include "OperationExecutor.hpp"
#include <functional>
#include "Server.hpp"
#include "DumpDBToFile.hpp"
#include "Logger.hpp"

using namespace std;

int main()
{
    Logger::clearLogs();
    Logger::saveToFile("InfoDataBase Application started");
    DumpDBToFile::clearLogs();
    MainContainer* mc = new MainContainer();
    OperationExecutor* oe = new OperationExecutor(mc);


    Server* server = new Server(oe);
    if(!server->Init())
    {
        server->listenSocket();
        server->start();
    }

    return 0;
}


