#ifndef OBSERVERMANAGER_H
#define OBSERVERMANAGER_H
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "MoBase.hpp"
#include <memory>
#include "Logger.hpp"

using namespace std;

class ObserverManager
{
    public:
        typedef MoBase* MoBasePtr;
        ObserverManager();
        virtual ~ObserverManager();
        int addObservers(int subscriptorId, string observedObject);
        int addObservers(int subscriptorId, string observedObject, string* statePtr, string value);
        void deleteObserver(int i);
        int findObserver(string distname);
        size_t getSize();
        vector<string> observedObjects_;
        vector<string*> observedStates_;
        vector<string> expectedValues_;
        vector<int> observers_;
    private:
        int getLastAddedObserverIndex();


};

#endif // OBSERVERMANAGER_H
