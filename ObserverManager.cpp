#include "ObserverManager.hpp"
#include "DumpDBToFile.hpp"
ObserverManager::ObserverManager()
{
   // Logger::saveToFile("ObserverManager/INF: ctor");

}

ObserverManager::~ObserverManager()
{
    //Logger::saveToFile("ObserverManager/INF: destructor");
}

int ObserverManager::getLastAddedObserverIndex()
{
    return observers_.size() - 1;
}

size_t ObserverManager::getSize()
{
    return observedObjects_.size();
}
int ObserverManager::addObservers(int subscriptorId, string observedObject)
{
    //Logger::saveToFile("ObserverManager/INF: addObservers for subscriptor: " + to_string(subscriptorId) + " on object: " + observedObject);
    DumpDBToFile::saveToFile("OBSERVER-" + to_string(subscriptorId) + "-"+ observedObject);
    observers_.push_back(subscriptorId);
    int index = getLastAddedObserverIndex();
    observedObjects_.push_back(observedObject);
    observedStates_.push_back(nullptr);
    expectedValues_.push_back("");
    return index;
}
int ObserverManager::addObservers(int subscriptorId, string observedObject, string* statePtr, string value)
{
    //Logger::saveToFile("ObserverManager/INF: addObservers for subscriptor: " + to_string(subscriptorId) + " on object: " + observedObject + " for value: " + value);
    DumpDBToFile::saveToFile("OBSERVER-" + to_string(subscriptorId) + "-"+ observedObject +"-"+value);
    observers_.push_back(subscriptorId);
    int index = getLastAddedObserverIndex();
    observedObjects_.push_back(observedObject);
    observedStates_.push_back(statePtr);
    expectedValues_.push_back(value);
    return index;

    //cout << "ObserverManager: addOservers: observedObject size: "<< observedObjects_[index] << endl;
    //cout << "ObserverManager: addOservers: observedState size: "<< observedStates_[index] << endl;
    //cout << "ObserverManager: addOservers: expectedValues size: "<< expectedValues_[index] << endl;

}

void ObserverManager::deleteObserver(int i)
{
    Logger::saveToFile("ObserverManager/INF: deleting observer for object: " + observedObjects_[i]);
    DumpDBToFile::saveToFile("DELETE: OBSERVER-"+ observedObjects_[i]);
    observedObjects_.erase(observedObjects_.begin() + i);
    observedStates_.erase(observedStates_.begin() + i);
    expectedValues_.erase(expectedValues_.begin() + i);
    observers_.erase(observers_.begin() + i);
}



int ObserverManager::findObserver(string name)
{

}
