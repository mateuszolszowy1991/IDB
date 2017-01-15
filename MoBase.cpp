#include "MoBase.hpp"

MoBase::MoBase(string distname) : distname_(distname)
{
    Logger::saveToFile("MoBase/INF: ctor " + distname);
    operationalState_="new";
    proceduralState_="new2";

    //cout << "MoBase/DBG: operationalStatePtr: " << &operationalState_ << endl;
    //cout << "MoBase/DBG: proceduralStatePtr: " << &proceduralState_ << endl;
    DumpDBToFile::saveToFile(distname_+"\n\toperationalState: "+operationalState_+"\n\tproceduralState: "+proceduralState_+"\n");
}

MoBase::~MoBase()
{
    Logger::saveToFile("MoBase/INF: deleting object " + distname_);
}

MoBase* MoBase::findInChildren(string distname)
{

    Logger::saveToFile("MoBase: " + distname_ + " findInChildren: looking for:" + distname);
    for(const auto &child : children_)
    {
        //cout <<"MoBase/DBG: child: #" << child->getDistname() << "# looking for: #" << distname << "#"<< endl;
        //cout << "!!!: " << child->getDistname().find(distname, distname.size())  << endl;
        if(child->getDistname().find(distname) != child->getDistname().npos)
        {
            //cout << "MoBase/DBG: child "<< child->getDistname()<<" has been found: "  << endl;
            return child;
        }
    }
    //cout << "MoBase/DBG: child has been not found" <<endl;
    return nullptr;
}

MoBase* MoBase::addChild(string distname)
{
    //cout <<"MoBase: " << distname_ << " AddChild: " << distname<< endl;
    children_.push_back(new MoBase(distname));
    //cout << "MoBase/DBG: "<< distname_ << " sizeof children_: " << children_.size() << " incresed by " << distname << endl;
    //cout << "MoBase/DBG: last child distname: " << (children_.back())->getDistname() << endl;
    return children_.back();
}


void MoBase::deleteChild(string distname)
{
    for(vector<MoBase*>::iterator i = children_.begin(); i < children_.end(); i++)
    {
        if((*i)->getDistname() == distname)
        {
           children_.erase(i);
        }

    }


}
string MoBase::setState(string* statePtr, string value)
{
    //Logger::saveToFile("MoBase/INF: "+distname_+" setState ");
    *statePtr = value;
    DumpDBToFile::saveToFile(distname_+"\n\toperationalState: "+operationalState_+"\n\tproceduralState: "+proceduralState_+"\n");
    //cout << "MoBase/DBG: setState: " << statePtr << "set to: " << *statePtr <<  endl;
    return value;
}
bool MoBase::expand(string detailName, string detailValue)
{
    for(auto &detail : details_)
    {
        if(detail.first == detailName)
        {
            if(detail.second != detailValue)
            {
                detail.second = detailValue;
                string txt = distname_+"\n\toperationalState: "+operationalState_+"\n\tproceduralState: "+proceduralState_+"\n";
                for(const auto &detail : details_)
                {
                    txt += "\t" + detail.first + ":\t" + detail.second + "\n";
                }
                DumpDBToFile::saveToFile(txt);
                //Logger::saveToFile("MoBase/INF: expand: " + txt);
                return true;
            }

        }
    }
    details_.push_back(make_pair(detailName, detailValue));
    string txt = distname_+"\n\toperationalState: "+operationalState_+"\n\tproceduralState: "+proceduralState_+"\n";
    for(const auto &detail : details_)
    {
        txt += "\t" + detail.first + ":\t" + detail.second + "\n";
    }
    DumpDBToFile::saveToFile(txt);
    return true;
}

string MoBase::getExtend(string detailName)
{
    for(const auto &detail : details_)
    {
        if(detail.first == detailName)
        {
            return detail.second;
        }
    }
    return "NotExists";
}




