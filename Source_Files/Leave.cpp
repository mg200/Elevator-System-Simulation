//#include "Event.h"
#include "Leave.h"
//#include"Hospital.h"
//#include<iostream>


Leave::Leave(Hospital* h, int ET,int ID):Event(h,ET,ID) {
	this->setType('L');
	this->setET(ET);
}


void Leave::Execute()  {
	const int IDofLeave = this->getID();
	Floor**list = HospitalManager->getListOfFloors();
	Floor*temp=list[0]; //pointer on the ground floor
//we assume Leave event could only work for visitors

	Stack<Pickable*>hold;
	
	Queue<Pickable*>*groundvisitors=temp->getUpVisitors();
	Pickable* Leaving;
	int existingcount = temp->getUpVisitors()->getCount();
	for (int i = 0; i < existingcount; i++) {
		Pickable* picked=groundvisitors->dequeue();
		int hisid = picked->getPickableId();
		if (picked->getPickableId() != IDofLeave) {
			hold.push(picked);
		}
		if (picked->getPickableId() == IDofLeave) {
			Leaving = picked;
			HospitalManager->leaving.enqueue(Leaving);
			Leaving->Left = true;
			HospitalManager->RemoveFromInservice(Leaving->getPickableId());
            //really not sure about this
			char typ=Leaving->getType();
			if (typ == 'V') HospitalManager->CascadeRequest(Leaving,HospitalManager->getVisitorElevator());
			if (typ == 'P') HospitalManager->CascadeRequest(Leaving,HospitalManager->getPatientElevator());
			if (typ == 'C') HospitalManager->CascadeRequest(Leaving,HospitalManager->getCargoElevator());
			while (!groundvisitors->isEmpty()) {
				picked=groundvisitors->dequeue();
				hold.push(picked);
			}
			while (!hold.isEmpty()) {
				hold.pop(picked);
				groundvisitors->enqueue(picked);
			}
			return;
		}
	}
}


void Leave::setET(int ET) {
	this->ET = ET;
}
void  Leave::setID(int ID) {
	//this->ID = ID;
}	
int Leave::getID()const {
	return this->ID;
}
int  Leave::getET()const {

	return this->ET;
}

//Leave::~Leave() {
//
//}
void Leave::setType(char type) {
	this->EventType = type;
}

void Leave::Print(std::ostream& out) const {
	out << this->EventType << " " << this->getET() << " "<<this->getID()<<endl;
}