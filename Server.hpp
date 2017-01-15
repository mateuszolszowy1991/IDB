#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>

#include <iostream>
#include "OperationExecutor.hpp"
#include "DumpDBToFile.hpp"
#include "Logger.hpp"
#include "Queue.hpp"
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include "stringOperations.hpp"
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE   1
#define FALSE  0
#define PORT 12345

using namespace std;

class Server
{
    public:
        Server(OperationExecutor* oE);
        virtual ~Server();
        int Init();
        void listenSocket();
        void start();
    private:
        int opt = TRUE;
        int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
        int max_sd;
        struct sockaddr_in address;
        char *buffer;
        char *message = "IDB v1.0; Welcome \r\n";
        int MAXRECV = 5000;
        fd_set readfds;

        OperationExecutor* oE_;
        MainContainer* mC_;
        Queue* queueService_;
        string getNotificationFromResult(string result);
        int getSubscriptorIdFroResult(string result);
        vector<string> splitString(string result, char splitBy);
};

#endif // SERVER_H
