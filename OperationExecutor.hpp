#ifndef OPERATIONEXECUTOR_H
#define OPERATIONEXECUTOR_H
#include "Messenger.hpp"
#include "MainContainer.hpp"
#include "FaultApplicationRecovery.hpp"
#include <functional>

using namespace std;

class OperationExecutor
{
    public:
        OperationExecutor(MainContainer* mc);
        string execute(string message, int subscriptorId);
    protected:

    private:
        Messenger* messenger_;
        MainContainer* mC_;
        FaultApplicationRecovery* far_;

};

#endif // OPERATIONEXECUTOR_H
