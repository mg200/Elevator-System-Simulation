#pragma once
#include"Event.h"
#include"Hospital.h"

using namespace std;
class Event;
//class Hospital;
class Leave :
    public Event
{
protected:
    void Print(std::ostream&) const;

public:
    Leave(Hospital* h, int ET,int ID);
    virtual void Execute();
    //virtual void Execute(Hospital* HospitalManager);
     void setET(int ET);
     void setID(int ID);
      void setType(char);
     int getID()const;
     int getET()const;
     ~Leave();
};

