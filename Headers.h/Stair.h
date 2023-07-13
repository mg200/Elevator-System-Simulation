#pragma once
#include "Event.h"
#include"Hospital.h"

//#include<iostream>
//#include"UI.h"
//#include<string>
//#include"Queue.h"
//#include"Pickable.h"

//class Hospital;
class Event;
class Stair :
    public Event
{
private:
    Queue<Pickable*>pickables;
    int countOfPickables;

protected:
    void Print(std::ostream&) const;
public:

    Stair(Hospital* h, int ET, int ID);
    virtual void Execute();
    //virtual void Execute(Hospital* HospitalManager);

    //friend std::ostream& operator<<(std::ostream& COUT, const Stair& s1) {
    //    //COUT << s1.id;               
    //    return COUT;
    //}                                 
    //

     void setType(char);

};

