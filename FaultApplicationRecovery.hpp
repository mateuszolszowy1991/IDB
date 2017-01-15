#ifndef FAULTAPPLICATIONRECOVERY_H
#define FAULTAPPLICATIONRECOVERY_H
#include <iostream>
#include <string>
#include <vector>
#include "MainContainer.hpp"
#include "CommonTools/stringOperations.hpp"
#include "Logger.hpp"


class FaultApplicationRecovery
{
    public:
        FaultApplicationRecovery(MainContainer* mc);
        virtual ~FaultApplicationRecovery();
        void checkRulesForFaultsRecovery(string bodyMessage);
    protected:

    private:
        MainContainer* mC_;
        void checkDOORSRules();
        bool checkIsDoorOpened();
        bool checkIsDoorClosed();
        void checkRCRules();
        bool checkLowBatteryRule();
        vector<string> raisedAlarmsAndNotifications_;

};

#endif // FAULTAPPLICATIONRECOVERY_H
