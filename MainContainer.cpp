#include "MainContainer.hpp"

MainContainer::MoBasePtr MainContainer::createRootObject(string distname)
{
    Logger::saveToFile("MainContainer/INF: object " + distname + " has been created as root object");
    db_.push_back(new MoBase(distname));
    return db_.back();
}
int MainContainer::checkStatesForObservers(string distname, string* statePtr, string value)
{
    //cout << "CheckStateForObservers for: " << distname <<endl;
    int j = 0;
    for(vector<string>::iterator i = observerManager_->observedObjects_.begin(); i < observerManager_->observedObjects_.end(); i++)
    {
       // cout << "Get: " << (*i) << " expected: " << distname <<endl;
        if((*i) == distname)
        {
            if(statePtr == nullptr)
            {
                observerManager_->deleteObserver(j);
                return j;
            }
            else
            {
                //cout <<"distname is found"<<endl;
                if(getObjectPtrByDistname(distname)->getState(statePtr) == value)
                {
                    //cout << "expected value is set sending to: "<< j <<endl;
                    observerManager_->deleteObserver(j);
                    return j;
                }
            }

        }
        j++;
    }
    return -1;
}


MainContainer::MoBasePtr MainContainer::createObject(string distname)
{
    if(checkIfIsTopLevel(splitXPath(distname).size()))
    {
        //cout << "MainContainer/DBG: createObject: rootObject creating"<< endl;
        return createRootObject(distname);
    }
    else
    {
        //cout << "MainContainer/DBG: createObject: childObject creating"<< endl;
        return getObjectPtrByDistname(distname)->addChild(distname);
    }
}

bool MainContainer::deleteObject(string distname)
{
    DumpDBToFile::saveToFile("DELETED: " + distname+"\n");
    MoBasePtr mo = getParentPtrByDistname(distname);
    if(mo)
    {
        mo->deleteChild(distname);
        return true;
    }
    return false;
}

string MainContainer::getState(string distname, string* statePtr)
{

}

MainContainer::MoBasePtr MainContainer::getObjectPtrByDistname(string distname)
{
    vector<string> splittedXPath = splitXPath(distname);
    size_t levels = splittedXPath.size();
    if(checkIfIsTopLevel(levels))
    {
        //cout << "MainContainer/DBG: getObjectPtrByDistname: find single distname" << endl;
        return getPtrToRootObject(distname);
    }
    else
    {
//       cout << "MainContainer/DBG: getObjectPtrByDistname: found complex distname" << endl;
       MainContainer::MoBasePtr root = getPtrToRootObject(splittedXPath[0]);
 //      cout << "MainContainer/DBG: getObjectPtrByDistname: starting from object: " << root->getDistname() << endl;
       if(root)
       {
           for(vector<string>::iterator i = splittedXPath.begin()+1; i < splittedXPath.end(); i++)
           {
               //cout << "MainContainer/DBG: getObjectPtrByDistname: Checking child in: "<< root->getDistname() << "object, looking for " << *i << endl;
               MainContainer::MoBasePtr child = root->findInChildren(*i);
               //cout <<"MOST IMPORTANT: child : "<<child <<endl;
               if(child && i != splittedXPath.end() - 1)
               {
                   //cout << "MainContainer/DBG: getObjectPtrByDistname: child is found and path is not end, continue..." <<endl;
                   root = child;
               }
               else if(child && i == splittedXPath.end() - 1)
               {
                   //cout << "MainContainer/DBG: getObjectPtrByDistname: child is not nullptr and path is end" << child->getDistname() <<endl;
                   return child;
               }
               else if(!child && i == splittedXPath.end() - 1)
               {
                   //cout << "MainContainer/DBG: getObjectPtrByDistname: child is nullptr and path is end" << root->getDistname() <<endl;
                   return root;
               }

            }
       }
       //cout << "return nullptr" <<endl;
       return nullptr;
    }

}

MainContainer::MoBasePtr MainContainer::getParentPtrByDistname(string distname)
{
    vector<string> splittedXPath = splitXPath(distname);
    size_t levels = splittedXPath.size();
    if(checkIfIsTopLevel(levels))
    {
        //cout << "MainContainer/DBG: getObjectPtrByDistname: find single distname" << endl;
        return nullptr;
    }
    else
    {
       //cout << "MainContainer/DBG: getObjectPtrByDistname: found complex distname" << endl;
       MainContainer::MoBasePtr root = getPtrToRootObject(splittedXPath[0]);
       //cout << "MainContainer/DBG: getObjectPtrByDistname: starting from object: " << root->getDistname() << endl;
       if(root)
       {
           for(vector<string>::iterator i = splittedXPath.begin()+1; i < splittedXPath.end(); i++)
           {
               //cout << "MainContainer/DBG: getObjectPtrByDistname: Checking child in: "<< root->getDistname() << "object, looking for " << *i << endl;
               MainContainer::MoBasePtr child = root->findInChildren(*i);
               //cout <<"MOST IMPORTANT: child : "<<child <<endl;
               if(child && i != splittedXPath.end() - 1)
               {
                   //cout << "MainContainer/DBG: getObjectPtrByDistname: child is found and path is not end, continue..." <<endl;
                   root = child;
               }
               else if(child && i == splittedXPath.end() - 1)
               {
                   //cout << "MainContainer/DBG: getObjectPtrByDistname: child is not nullptr and path is end" << child->getDistname() <<endl;
                   return root;
               }
               else if(!child && i == splittedXPath.end() - 1)
               {
                   //cout << "MainContainer/DBG: getObjectPtrByDistname: child is nullptr and path is end" << root->getDistname() <<endl;
                   return nullptr;
               }

            }
       }
       //cout << "return nullptr" <<endl;
       return nullptr;
    }

}

MainContainer::MoBasePtr MainContainer::checkIsObjectExist(string distname)
{
    vector<string> splittedXPath = splitXPath(distname);
    size_t levels = splittedXPath.size();
    if(checkIfIsTopLevel(levels))
    {
        //cout << "MainContainer/DBG: getObjectPtrByDistname: find single distname" << endl;
        return getPtrToRootObject(distname);
    }
    else
    {
       //cout << "MainContainer/DBG: getObjectPtrByDistname: found complex distname" << endl;
       MainContainer::MoBasePtr root = getPtrToRootObject(splittedXPath[0]);
       //cout << "MainContainer/DBG: getObjectPtrByDistname: starting from object: " << root->getDistname() << endl;
       if(root)
       {
           for(vector<string>::iterator i = splittedXPath.begin()+1; i < splittedXPath.end(); i++)
           {
               //cout << "MainContainer/DBG: getObjectPtrByDistname: Checking child in: "<< root->getDistname() << "object, looking for " << *i << endl;
               MainContainer::MoBasePtr child = root->findInChildren(*i);

               if(child && i != splittedXPath.end() - 1)
               {
                   //cout << "MainContainer/DBG: getObjectPtrByDistname: child is found and path is not end, continue..." <<endl;
                   root = child;
               }
               else if(!child && i == splittedXPath.end() - 1)
               {
                   return nullptr;
               }
               else if(child->getDistname() == *i)
               {
                   return child;
               }
            }
       }
    }

}

MainContainer::MoBasePtr MainContainer::getPtrToRootObject(string distname)
{
    for(vector<MoBasePtr>::iterator i = db_.begin(); i < db_.end(); i++)
    {
        if((*i)->getDistname() == distname)
        {
            return *i;
        }
        else
        {
            return nullptr;
        }
    }

}

int MainContainer::setObserver(int subscriptorId, string distname, string* statePtr, string value)
{
    //cout << "MainCOntainer: setObserver " <<distname << " statePtr " << statePtr << " value " << value <<endl;
    MoBase* object = getObjectPtrByDistname(distname);
    //cout << object <<endl;
    if(object->getDistname() == distname)
    {

        if(statePtr == nullptr)
        {

            //cout << "StatePtr is nullptr, so we return subscriptorId, because object exists"<<endl;
            //cout << "Subscriptor identifier: " << subscriptorId <<endl;
            return subscriptorId;
        }
        else
        {
            //cout << "OBJECT DISTNAME " << object->getDistname() <<  "statePTr: " << statePtr <<endl;
            string currentState = object->getState(statePtr);
            //cout << "MainContainer/DBG: setObserver: currentState: " << currentState << " value " << value << endl;
            if(currentState == value)
            {
                //cout << "Subscriptor identifier: " << subscriptorId <<endl;
                return subscriptorId;
            }
            else
            {
                return -1;
            }
        }

    }
    else
    {
        observerManager_->addObservers(subscriptorId, distname, statePtr, value);
        return -1;
    }


}

void MainContainer::setDetail(string objectDistname, string detailName, string detailValue)
{
    auto objectPtr = getObjectPtrByDistname(objectDistname);
    objectPtr->expand(detailName, detailValue);
}

string MainContainer::serializeObject(MainContainer::MoBasePtr objectPtr)
{
    string operationalStatePtrStr = to_string(reinterpret_cast<uintptr_t>(objectPtr->getOperationalStatePtr()));
    string proceduralStatePtrStr = to_string(reinterpret_cast<uintptr_t>(objectPtr->getProceduralStatePtr()));
    return objectPtr->getDistname()+";"+operationalStatePtrStr+";"+proceduralStatePtrStr;
}
//0x6-[binary]-[MoPtr];[detailName];[detailValue]


MainContainer::RESULT2 MainContainer::deserializeMoPtrAndDetails(string str)
{
    vector<string> splittedPtrs = splitPtrs(str);
    if(splittedPtrs.size() == 3)
    {
        MainContainer::RESULT2 result = {splittedPtrs[0], splittedPtrs[1], splittedPtrs[2]};
        return result;
    }
    else if(splittedPtrs.size() == 2)
    {
        MainContainer::RESULT2 result = {splittedPtrs[0], splittedPtrs[1], ""};
        return result;
    }
}

MainContainer::RESULT MainContainer::deserializeObject(string strPtrs)
{
    //Logger::saveToFile("MainContainer/DBG: strPtrs " + strPtrs);
    string* firstStatePtr;
    vector<string> splittedPtrs = splitPtrs(strPtrs);
    if(splittedPtrs.size() > 1)
    {
        uintptr_t firstIntPtr = atoi(splittedPtrs[1].c_str());
        firstStatePtr = reinterpret_cast<string*>(firstIntPtr);
    }

    //cout << "MainContainer/DBG: deserializeObject: firstPointer: " << firstStatePtr <<endl;
    if(splittedPtrs.size() == 2)
    {
       //Logger::saveToFile("PTRS: 2");
       MainContainer::RESULT result = {splittedPtrs[0], firstStatePtr, nullptr, " "};
       //result.show();
       return result;
    }
    else if(splittedPtrs.size() == 3)
    {
       //Logger::saveToFile("PTRS: 3");
       MainContainer::RESULT result = {splittedPtrs[0], firstStatePtr, nullptr, splittedPtrs[2]};
       //result.show();
       return result;
    }
    if(splittedPtrs.size() == 1)
    {
        Logger::saveToFile("PTR: " + splittedPtrs[0]);
        MainContainer::RESULT result = {splittedPtrs[0], nullptr, nullptr,""};
        //result.show();
        return result;
    }
    //Logger::saveToFile("PTRS: 0");
    MainContainer::RESULT result = {"", nullptr, nullptr, ""};
    return result;

}

bool MainContainer::checkIfIsTopLevel(size_t levels)
{
    // cout << "LEVELS: " << levels <<endl;
    if(levels == 1)
    {
        return true;
    }
    return false;
}

vector<string> MainContainer::splitXPath(string distname)
{
    for (int i=0; i < distname.length(); i++)
    {
        if (distname[i] == '/')
            distname[i] = ' ';
    }
    vector<string> splitted;
    stringstream ss(distname);
    string temp;
    while (ss >> temp)
    {
        splitted.push_back(temp);
    }
    return splitted;
}

vector<string> MainContainer::splitPtrs(string strPtr)
{
    for (int i=0; i < strPtr.length(); i++)
    {
        if (strPtr[i] == ';')
            strPtr[i] = ' ';
    }
    vector<string> splitted;
    stringstream ss(strPtr);
    string temp;
    while (ss >> temp)
    {
        splitted.push_back(temp);
    }
    return splitted;
}
