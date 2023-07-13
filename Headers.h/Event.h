#pragma once
#ifndef	EVENT_H
#define EVENT_H
#include"Hospital.h"
#include<iostream>
#include"Queue.h"
#include"Pickable.h"
#include"Stack.h"
#include<iostream>
class Hosptial;

class Event
{
private:


protected:
	Hospital* HospitalManager=nullptr;
	char EventType;
	const int ID;
	int ET;//event timestep
	//Hospital* HospitalManager = nullptr;
	virtual void Print(std::ostream&) const = 0;

public:
	int emrlvl;
	virtual void Execute() = 0;
	//virtual void Execute(Hospital* HospitalManager) = 0;
	Event(Hospital*h,int ET, int ID, int emrlevel = 0) :ID{ ID } {
		this->setET(ET);
		this->emrlvl = emrlevel;
		//this->setID(id);
		HospitalManager = h;
	}

	virtual void setType(char type) { this->EventType = type; }
	virtual char getType()const{ return EventType; }
	virtual void setET(int et){ this->ET = et; }
	//virtual void setID(int ID);
	virtual int getID()const { return ID; }
	virtual int getET()const {
		return ET;
	}
	
	//friend std::ostream& operator<<(std::ostream& COUT, Event& e1);
	friend std::ostream& operator<<(std::ostream& COUT, Event& e1) {
		e1.Print(COUT);	//this function is defined with the Print function above to enable us to
		//overload the ostream operator so we can custom print each event
		return COUT;
	}
	//~Event();
};											 



#endif

