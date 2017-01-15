#include "OperationExecutor.hpp"

OperationExecutor::OperationExecutor(MainContainer* mc) : messenger_(new Messenger()),
                                                          mC_(mc),
                                                          far_(new FaultApplicationRecovery(mc))
{
    //Logger::saveToFile("OperationExecutor/INF: ctor: ");
}

string OperationExecutor::execute(string message, int subscriptorId)
{

    messenger_->parseMessage(message);

    //cout << "OperationExecutor/DBG: message parsed. Header:" << message << endl;

    if(messenger_->getHeader() == "0x0")
    {
        Logger::saveToFile("OperationExecutor/INF: Welcome message has been recognised");
        string msg = "0x0-IDB-0x0-welcome;0x1:create;0x2:getState;0x3:setState;0x4:deleteObject;0x5:setObserver;0x6:expand;0x7:isExist;0x8:getObject;0x9:getDetailValue";
        return msg;
    }
    else if(messenger_->getHeader() == "0x1")
    {
        Logger::saveToFile( "OperationExecutor/INF: execute creatingObject" );
        MoBase* object = mC_->createObject(messenger_->getBody());
        string response = mC_->serializeObject(object);
        //cout << "CREATING OBJECT RESULT: " << response << endl;
        int subscriptorId = mC_->checkStatesForObservers(messenger_->getBody(), nullptr, "");
        if(subscriptorId >= 0)
        {
            //cout << "MainContainer/DBG: in something way we need send notification that observer has been found: " << subscriptorId << endl;
            ostringstream ss;
            ss << subscriptorId;
            string str = ss.str();
            return "0x1-IDB-" + response+"#" + str;
        }
        return "0x1-IDB-" + response;//msg;
    }
    else if(messenger_->getHeader() == "0x2")
    {
        Logger::saveToFile("OperationExecutor/INF: execute getState" );
        auto deserializedObject = mC_->deserializeObject(messenger_->getBody());

        //cout << "OperationExecutor/DBG: expected statePtr: " << (pairedObject.second)[0] << " is equal: " << *(pairedObject.second)[0] << endl;
        //cout << "OperationExecutor/DBG: expected ObjectPtr: " << pairedObject.first << endl;
        auto mo = mC_->getObjectPtrByDistname(deserializedObject.distname);
        if(mo != nullptr)
        {
            return "0x2-IDB-"+(mC_->getObjectPtrByDistname(deserializedObject.distname))->getState(deserializedObject.statePtr);// + *(pairedObject.statePtr);
        }


    }
    else if(messenger_->getHeader() == "0x3")
    {
       Logger::saveToFile("OperationExecutor/INF: execute setState: " + messenger_->getBody());
        //cout << "DEBUG: " << messenger_->getBody() <<endl;
        auto deserializedObject = mC_->deserializeObject(messenger_->getBody());
        //cout << "NO I CO: " << mC_->getObjectPtrByDistname(deserializedObject.distname) <<endl;
        string value = (mC_->getObjectPtrByDistname(deserializedObject.distname))->setState(deserializedObject.statePtr, deserializedObject.value);
        //cout << "RETURNED DEBUG VALUE: " << value <<endl;
        int subscriptorId = mC_->checkStatesForObservers(deserializedObject.distname, deserializedObject.statePtr, deserializedObject.value);
        far_->checkRulesForFaultsRecovery(messenger_->getBody());
        if(subscriptorId >= 0)
        {
            //cout << "MainContainer/DBG: in something way we need send notification that observer has been found: " << subscriptorId << endl;
            ostringstream ss;
            ss << subscriptorId;
            string str = ss.str();
            return "0x3-IDB-" +value + "#" + str;
        }
        return "0x3-IDB-"+value;

    }
    else if(messenger_->getHeader() == "0x4")
    {
        Logger::saveToFile("OperationExecutor/INF: execute deleteObject");
        far_->checkRulesForFaultsRecovery(messenger_->getBody());
        if(mC_->deleteObject(messenger_->getBody()))
        {
            return "0x4-IDB-true";
        }
        else
        {
            return "0x4-IDB-false";
        }


    }
    else if(messenger_->getHeader() == "0x5")
    {
        int subscriptionId;
        Logger::saveToFile("OperationExecutor/INF: execute setObserver: " );
        auto deserializedObject = mC_->deserializeObject(messenger_->getBody());
        //Logger::saveToFile("Deseriaized: " + deserializedObject.statePtr)
        if(deserializedObject.statePtr != nullptr)
        {
            Logger::saveToFile("Set Observer for value change");
            subscriptionId = mC_->setObserver(subscriptorId, deserializedObject.distname, deserializedObject.statePtr, deserializedObject.value);
        }
        else
        {
            Logger::saveToFile("Set Observer for creation");
            subscriptionId = mC_->setObserver(subscriptorId, deserializedObject.distname, nullptr, "");
        }
        //Logger::saveToFile("SOMETHING WRONG?");

        if(subscriptionId >= 0)
        {
            //cout << "MainContainer/DBG: in something way we need send notification that observer has been found: " << subscriptorId << endl;
            ostringstream ss;
            ss << subscriptionId;
            string str = ss.str();
            string serializedObject = mC_->serializeObject(mC_->getObjectPtrByDistname(deserializedObject.distname));
            //cout <<"OperationExecutor/DBG: serilized, send to server to forward: " << "0x5-IDB-#"+str+";"+deserializedObject.distname +";"+serializedObject << endl;
            return "0x5-IDB-#"+str+";"+serializedObject;
        }
        else if (subscriptionId == -1)
        {
            Logger::saveToFile("SET");
            return "0x5-IDB-set";
        }
        return "0x5-IDB-true";
    }
    else if(messenger_->getHeader() == "0x6")
    {
        Logger::saveToFile("OperationExecutor/INF: execute expand" );
        far_->checkRulesForFaultsRecovery(messenger_->getBody());
        auto deserialized = mC_->deserializeMoPtrAndDetails(messenger_->getBody());
        MoBase* objectPtr = mC_->getObjectPtrByDistname(deserialized.distname);
        if(objectPtr->expand(deserialized.detailName, deserialized.detailValue))
        {
            return "0x6-IDB-true";
        }

    }
    else if(messenger_->getHeader() == "0x7")
    {
        Logger::saveToFile("OperationExecutor/INF: execute checkIsExist" );
        auto deserializedObject = mC_->deserializeObject(messenger_->getBody());
        auto mo = mC_->getObjectPtrByDistname(deserializedObject.distname);
        Logger::saveToFile("OperationExecutor: " + mo->getDistname());
        if(mo->getDistname().find(deserializedObject.distname) != mo->getDistname().npos)
        {
            return "0x7-IDB-true";
        }
        else
        {
            return "0x7-IDB-false";
        }

    }
    else if(messenger_->getHeader() == "0x8")
    {
        Logger::saveToFile("IDB/INF: OperationExecutor: execute getObject " + messenger_->getBody());
        auto mo = mC_->getObjectPtrByDistname(messenger_->getBody());

        if(mo->getDistname() == messenger_->getBody())
        {
            string response = mC_->serializeObject(mo);
            return "0x8-IDB-" + response;
        }
        else
        {
            return "0x8-IDB-0";
        }

    }
    else if(messenger_->getHeader() == "0x9")
    {
        Logger::saveToFile("IDB/INF: OperationExecutor: execute getDetailValue " + messenger_->getBody());
        auto mo = mC_->getObjectPtrByDistname(splitString(messenger_->getBody(), ';')[0]);
        Logger::saveToFile("IDB/DBG: GETBODY: " + splitString(messenger_->getBody(), ';')[0]);
        Logger::saveToFile("IDB/DBG: GETDISTNAE: " + mo->getDistname());
        if(mo->getDistname() == splitString(messenger_->getBody(), ';')[0])
        {
            Logger::saveToFile("IDB/INF: OperationExecutor: objectdistname: " + mo->getDistname());
            Logger::saveToFile("IDB/INF: OperationExecutor: getBody?: " + splitString(messenger_->getBody(), ';')[1]);
            for(auto &detail : mo->getDetails())
            {
                cout << "DETAIL: " << detail.first << " VALUE: " << detail.second <<endl;
                if(detail.first == splitString(messenger_->getBody(), ';')[1])
                {
                    Logger::saveToFile("DETAIL VALUE: " + detail.second);
                    return "0x9-IDB-" + detail.second;
                    break;
                }
            }
        }
        else
        {
            return "0x9-IDB-0";
        }

    }

    messenger_->clearMessage();

    return "0";
}
