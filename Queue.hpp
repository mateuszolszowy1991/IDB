#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include <iostream>
#include <string>
#include "Logger.hpp"
using namespace std;
class Queue
{
    public:
        Queue();
        virtual ~Queue();
        void addTask(string task);
        string getTask();
        string getToCout();
        bool isNoTaskToExecute();
    protected:

    private:
        vector<string> tasks_;
};

#endif // QUEUE_H
