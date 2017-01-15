#include "FaultApplicationRecovery.hpp"

FaultApplicationRecovery::FaultApplicationRecovery(MainContainer* mc) : mC_(mc)
{
    Logger::saveToFile("IDB/INF: FaultApplicationRecovery: ctor");
}

FaultApplicationRecovery::~FaultApplicationRecovery()
{
    //dtor
}

void FaultApplicationRecovery::checkRulesForFaultsRecovery(string bodyMessage)
{
    Logger::saveToFile("IDB/DBG: FaultApplicationRecovery: checkRulesForFaultsRecovery: " + bodyMessage);
    vector<string> splittedMsg = splitString(bodyMessage, ';');
    if(splittedMsg.size() > 1)
    {
        if(splittedMsg[0] == "CAR/BDM/DOORS")
        {
            checkDOORSRules();
        }
        else if(splittedMsg[0] == "CAR/BDM/REMOTE_CONTROLLER")
        {
            Logger::saveToFile("IDB/DBG: FaultApplicationRecovery: checkRulesForFaultsRecovery: RC Rules will be checked");
            checkRCRules();
        }
    }
}

void FaultApplicationRecovery::checkDOORSRules()
{
    if(checkIsDoorOpened())
    {
        //cout << "DEBUG: " << mC_->getObjectPtrByDistname("CAR/FAR/NOTIF_DOORS_OPENED") << endl;
        if(!mC_->checkIsObjectExist("CAR/FAR/NOTIF_DOORS_OPENED"))
        {
            auto mo = mC_->createObject("CAR/FAR/NOTIF_DOORS_OPENED");
            raisedAlarmsAndNotifications_.push_back(mo->getDistname());
        }

    }
    if(checkIsDoorClosed())
    {
        mC_->deleteObject("CAR/FAR/NOTIF_DOORS_OPENED");
        auto it = find(raisedAlarmsAndNotifications_.begin(), raisedAlarmsAndNotifications_.end(), "CAR/FAR/NOTIF_DOORS_OPENED");
        raisedAlarmsAndNotifications_.erase(it);
        //Logger::saveToFile("FAR/INF: raised alarms count: "  raisedAlarmsAndNotifications_.size() <<endl;
    }
}

bool FaultApplicationRecovery::checkIsDoorOpened()
{
    auto mo = mC_->getObjectPtrByDistname("CAR/BDM/DOORS");
    return (mo->getProceduralState() == "open") ? true : false;

}

bool FaultApplicationRecovery::checkIsDoorClosed()
{
    auto mo = mC_->getObjectPtrByDistname("CAR/BDM/DOORS");
    return (mo->getProceduralState() == "close") ? true : false;
}

void FaultApplicationRecovery::checkRCRules()
{
    if(checkLowBatteryRule())
    {
        Logger::saveToFile("IDB/DBG: FaultApplicationRecovery: checkRCRules: batery is low");
        // << "DEBUG: " << mC_->getObjectPtrByDistname("CAR/FAR/ALARM_BATERY_LOW") << endl;
        if(!mC_->checkIsObjectExist("CAR/FAR/ALARM_BATERY_LOW"))
        {
            auto mo = mC_->createObject("CAR/FAR/ALARM_BATERY_LOW");
            raisedAlarmsAndNotifications_.push_back(mo->getDistname());
        }
    }
    else
    {
        if(mC_->checkIsObjectExist("CAR/FAR/ALARM_BATERY_LOW"))
        {
            Logger::saveToFile("IDB/DBG: FaultApplicationRecovery: checkRCRules: ALARm has been found");
            mC_->deleteObject("CAR/FAR/ALARM_BATERY_LOW");
            auto it = find(raisedAlarmsAndNotifications_.begin(), raisedAlarmsAndNotifications_.end(), "CAR/FAR/ALARM_BATERY_LOW");
            raisedAlarmsAndNotifications_.erase(it);
        }
    }
}

bool FaultApplicationRecovery::checkLowBatteryRule()
{
    Logger::saveToFile("IDB/DBG: FaultApplicationRecovery: checkLowBatteryRule");
    auto mo = mC_->getObjectPtrByDistname("CAR/BDM/REMOTE_CONTROLLER");
    //cout << "IDB/DBG: FaultApplicationRecovery: checkLowBatteryRule: "<< mo->getDistname() << " operationalState: " << mo->getOperationalState()<< endl;
    if(mo->getOperationalState() == "Low_Power")
    {
        Logger::saveToFile("IDB/DBG: FaultApplicationRecovery: checkLowBatteryRule: RemoteControllerDevice is low power");
        return true;
    }
    return false;
    //return (mo->getOperationalState() == "Low_Power") ? true : false;
}
