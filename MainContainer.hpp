#ifndef MAINCONTAINER_H
#define MAINCONTAINER_H
#include <iostream>
#include <string>
#include <memory>
#include <set>
#include "MoBase.hpp"
#include "ObserverManager.hpp"
#include "Messenger.hpp"
#include <sstream>
#include <algorithm>
#include "Logger.hpp"


using namespace std;

class MainContainer
{
    public:
    struct RESULT
    {
        string distname;
        string* statePtr;
        string* nextStatePtr;
        string  value;
        void show(){cout << "RESULT: Distname: " << distname << " statePtr: "<< statePtr << " nextStatePtr: " << nextStatePtr << " value: " << value <<endl;}
    };

    struct RESULT2
    {
        string distname;
        string detailName;
        string detailValue;
    };


        typedef MoBase* MoBasePtr;
        MainContainer() : observerManager_(new ObserverManager()){};
        virtual ~MainContainer(){};
        MoBasePtr createRootObject(string distname);
        MoBasePtr createObject(string distname);
        bool deleteObject(string distname);
        string getState(string distname, string* statePtr);
        void setDetail(string objectDistname, string detailName, string detailValue);


        int setObserver(int subscriptorId, string distname, string* statePtr, string value);
        int checkStatesForObservers(string distname, string* statePtr, string value);
        MainContainer::RESULT2 deserializeOnlyMoPtr(string str);
        string serializeObject(MainContainer::MoBasePtr objectPtr);
        RESULT deserializeObject(string strPtrs);
        RESULT2 deserializeMoPtrAndDetails(string str);
        MoBasePtr getObjectPtrByDistname(string distname);
        shared_ptr<Messenger> messenger_;

        MoBasePtr checkIsObjectExist(string distname);
    private:
        vector<string> splitXPath(string distname);
        vector<string> splitPtrs(string strPtrs);
        bool checkIfIsTopLevel(size_t levels);
        MoBasePtr getPtrToRootObject(string distname);
        MoBasePtr getParentPtrByDistname(string distname);


        vector<MoBasePtr> db_;
        ObserverManager* observerManager_;
};

#endif // MAINCONTAINER_H
