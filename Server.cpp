#include "Server.hpp"

Server::Server(OperationExecutor* oE) : oE_(oE), queueService_(new Queue())
{
    Logger::saveToFile("Server/INF ctor: memory allocation");
    buffer =  (char*) malloc((MAXRECV + 1) * sizeof(char));
    for(i = 0 ; i < 30;i++)
    {
        client_socket[i] = 0;
    }
}

Server::~Server()
{
    Logger::saveToFile("Server/ERR descriptor");
}

int Server::Init()
{
    Logger::saveToFile("Server/INF: Initialising Sock...");

    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    Logger::saveToFile("Server/INF: Socket created.");
//set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    return 0;
}

void Server::listenSocket()
{
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
    //Logger::saveToFile("Server/INF: Waiting for incoming connections...");

}

void Server::start()
{
    while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            //send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("send");
            }

            puts("Welcome message sent successfully");

            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the incoming message

                //Somebody disconnected , get his details and print
                getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                valread = recv( sd , buffer, 2000, 0);
                buffer[valread] = '\0';
                if(strlen(buffer) == 0)
                {
                    close(sd);
                    client_socket[i] = 0;
                }
                //Echo back the message that came in
                else
                {
                    queueService_->addTask(buffer);
                    //string str(buffer);
                    //Logger::saveToFile("Server/INF: received: " + str);
                    while(!queueService_->isNoTaskToExecute())
                    {
                        //cout << "is task to get and execute "<< queueService_->getToCout() <<  endl;
                        auto result = oE_->execute(queueService_->getTask(), i);

                        int subscriptorId = getSubscriptorIdFroResult(result);

                        if(result != "0")
                        {
                            Logger::saveToFile("Server/INF: Sending result: " + result);
                            send( sd, result.c_str() , strlen(result.c_str()) , 0 );
                            if (subscriptorId != -1)
                            {
                                //Logger::saveToFile("Server/INF: Sending notification about observer to subsrciber: " + to_string(subscriptorId));
                                int subscriptorSocket = client_socket[subscriptorId];
                                string notif = getNotificationFromResult(result);
                                notif = "0x5-IDB-" + notif ;
                                send(client_socket[subscriptorId] , notif.c_str() , strlen(notif.c_str()) , 0 );
                            }
                        }

                    }

                }

                if ( valread == 0)
                {
                    //Somebody disconnected , get his details and print
                    //printf("Server/INF: Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                    exit(-1);
                }
            }
        }
    }
}

int Server::getSubscriptorIdFroResult(string result)
{
    for (int i=0; i < result.length(); i++)
    {
        if (result[i] == '#')
            result[i] = ' ';
    }
    vector<string> splitted;
    stringstream ss(result);
    string temp;
    while (ss >> temp)
    {
        splitted.push_back(temp);
    }
    if(splitted.size() > 1)
    {
        return atoi(splitted[1].c_str());
    }
    return -1;
}

vector<string> Server::splitString(string result, char splitBy)
{
    for (int i=0; i < result.length(); i++)
    {
        if (result[i] == splitBy)
            result[i] = ' ';
    }
    vector<string> splitted;
    stringstream ss(result);
    string temp;
    while (ss >> temp)
    {
        splitted.push_back(temp);
    }
    return splitted;

}

string Server::getNotificationFromResult(string result)
{
    //Logger::saveToFile("Server/DBG: getNotificationFromResult: " + result);
    auto splitted = splitString(result, '-');
    return splitted[2];
}
