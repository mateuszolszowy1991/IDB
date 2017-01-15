#include "Messenger.hpp"

Messenger::Messenger()
{
    //ctor
}

Messenger::~Messenger()
{
   //cout << "Messenger/DBG: destruct"<< endl;
}

void Messenger::clearMessage()
{
    //cout << "Messenger/DBG: clearMessage()" << endl;
    message_ = "";
    header_ = "";
    from_ = "";
    body_ = "";
}

void Messenger::parseMessage(string msg)
{
    clearMessage();
    message_ = msg;
    header_ = getPole(0);
    from_ = getPole(1);
    body_ = getPole(2);
    //cout << "Messenger/DBG:  " << " Header: " << header_ << " From: " << from_ << " Body: " << body_<< endl;
}

string Messenger::getPoleFromBody(int pole)
{
    for (int i=0; i < body_.length(); i++)
    {
        if (body_[i] == ';')
            body_[i] = ' ';
    }
    vector<string> splitted;
    stringstream ss(body_);
    string temp;
    while (ss >> temp)
    {
        splitted.push_back(temp);
    }
    return splitted[pole];
}

string Messenger::getProperty()
{
    return getPoleFromBody(1);
}

string Messenger::getObject()
{
    return this->getPoleFromBody(0);
}

string Messenger::getValue()
{
    return this->getPoleFromBody(2);
}

string Messenger::getPole(int pole)
{
    for (int i=0; i < message_.length(); i++)
    {
        if (message_[i] == '-')
            message_[i] = ' ';

    }
    vector<string> splitted;
    stringstream ss(message_);
    string temp;
    while (ss >> temp)
    {
        splitted.push_back(temp);
    }
    return splitted[pole];
}

vector<string> Messenger::getVectorToPointers()
{
    string props = getPole(2);
    for (int i=0; i < props.length(); i++)
    {
        if (props[i] == ';')
            props[i] = ' ';
    }
    vector<string> splitted;
    stringstream ss(props);
    string temp;
    while (ss >> temp)
    {
        splitted.push_back(temp);
    }
    return splitted;
}

string Messenger::joinPtrsToMessage(vector<string> ptrs)
{
    string joinned = "";
    for(vector<string>::const_iterator i = ptrs.begin(); i != ptrs.end(); i++)
    {
        joinned += *i + ";";
    }

    return joinned;
}
