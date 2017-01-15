#ifndef MESSENGER_H
#define MESSENGER_H
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
class Messenger
{
    public:
        Messenger();
        virtual ~Messenger();
        void parseMessage(string msg);
        string getHeader(){return header_;}
        string getFrom(){return from_;}
        string getBody(){return body_;}
        string getProperty();
        string getObject();
        string getValue();
        string getPoleFromBody(int pole);
        void clearMessage();

        vector<string> getVectorToPointers();
        string joinPtrsToMessage(vector<string> ptrs);
    protected:

    private:
        string getPole(int pole);
        string message_;
        string header_;
        string from_;
        string body_;

};

#endif // MESSENGER_H
