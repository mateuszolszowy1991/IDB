#ifndef MOBASE_H
#define MOBASE_H
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "DumpDBToFile.hpp"
#include "Logger.hpp"

using namespace std;

class MoBase
{
public:
    typedef MoBase* MoBasePtr;

    MoBase(string distname);
    virtual ~MoBase();

    string* getProceduralStatePtr(){return &proceduralState_;}
    string getProceduralState(){return proceduralState_;}
    string setState(string* statePtr, string value);
    string getState(string* statePtr){ return *statePtr;}
    string* getOperationalStatePtr(){return &operationalState_;}
    string getOperationalState(){return operationalState_;}
    string getDistname(){return distname_;}
    bool expand(string datailName, string detailValue);
    string getExtend(string detailName);
    MoBase* findInChildren(string distname);
    MoBase* addChild(string distname);
    vector<MoBase*>* getPtrToChildren(){return &children_;}
    void deleteChild(string distname);
    vector<MoBase*> getChildren(){return children_;}
    vector<pair<string, string>> getDetails(){return details_;}

private:
    void dumpObjectToFile(string data);
    vector<MoBase*> children_;
    string distname_;
    string proceduralState_;
    string operationalState_;
    vector<pair<string, string>> details_;
};

#endif // MOBASE_H
