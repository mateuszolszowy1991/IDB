#include "Queue.hpp"

Queue::Queue()
{
    //ctor
}

Queue::~Queue()
{
    //dtor
}

void Queue::addTask(string task)
{
    tasks_.push_back(task);
    Logger::saveToFile("Queue/DBG: addTask: task " + task + " has been added to queue | size: " + to_string(tasks_.size()));
}

string Queue::getTask()
{
    string task = tasks_.front();
    tasks_.erase(tasks_.begin());
    return task;
}

string Queue::getToCout()
{
    string task = tasks_.front();
    return task;
}

bool Queue::isNoTaskToExecute()
{
    //cout << "Queue/DBG: TasksToExecute: " << tasks_.size() <<endl;
    if(tasks_.size() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}
