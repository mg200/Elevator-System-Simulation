#pragma once
#include "Event.h"
#include"Hospital.h"

#include<fstream>
//class Pickable;
// 
class Event;

//class Hospital;
class Arrival :
    public Event
{
         
protected:
     void Print(std::ostream&) const;

public:
    int sourcefloor, targetfloor, emrlevel;
    char Arrivaltype;
    bool Contagious_Case;
    Arrival(Hospital*h,char type, int timestep,int ID,int srcFloor, int trgtFloor,int=0,bool=false);
    //EMR_LVL is emergency level, a parameter for patients only
    //Contagious Case is also a patient-only parameter
     virtual void Execute();
     //virtual void Execute(Hospital* HospitalManager);
     void setET(int ET);
     void setID(int ID);
     void setType(char);
     void setArrivalType(char);
     char getArrivalType()const;
     void setSourceFloor(int);
     void setTargetFloor(int);
     int getID()const;
     int getET()const;
     int getSrcFloor()const;
     int getTrgtFloor()const;
    //~Arrival();
};

