#include "Arrival.h"
#include<cmath>
using namespace std;


Arrival::Arrival(Hospital* h, char type, int ET, int ID, int srcFloor, int trgtFloor, int EMR_LVL,bool isContagious): Event(h,ET,ID, EMR_LVL) {

	this->setType('A');
	this->emrlevel = EMR_LVL;
	this->setArrivalType(type);
	this->sourcefloor = srcFloor;
	this->targetfloor = trgtFloor;
	this->setET(ET);
	this->setID(ID);
	if (type == 'P' && isContagious)Contagious_Case = true;
}

void Arrival::Execute() {
	int i = this->getSrcFloor(), j = this->getTrgtFloor();
	int diff = j - i;				   
	if (diff==0)return;
	//a visitor cannot have the same source and target floor
	char type = this->getArrivalType();
	Pickable* newpickable = new Pickable(this->getArrivalType(), i, j, this->getID(), this->getET());
	newpickable->SetStairsCount(HospitalManager->getS());
	if (type=='P') {
		//PriorityEquation
int Priority = ceil( 0.7* this->emrlevel + 0.2 * this->getSrcFloor() + 0.3 * this->getTrgtFloor() - 0.2 * this->getET());
   //it is here that we populate the waiting lists, each timestep
		newpickable->waitinglistpriority = Priority;
		//Contagious Case
		if (this->Contagious_Case)Priority *= 1000;
		if(this->Contagious_Case)newpickable->ContagiousCase = true;
		if (diff > 0) {
			HospitalManager->getFloorwithID(i)->getUpPatients()->enqueue(newpickable, Priority);
			if (this->Contagious_Case)HospitalManager->getFloorwithID(i)->hasContagiousCase = true;
		}
		if(diff<0) {
	HospitalManager->getFloorwithID(i)->getDownPatients()->enqueue(newpickable,Priority);
	if (this->Contagious_Case)HospitalManager->getFloorwithID(i)->hasContagiousCase = true;
//	if (Contagious_Case)cout << " Source floor is " << i << " target floor " << j <<"of contagious" << endl;	
	}
		HospitalManager->getPatientElevator()->AddtoVisitList(i, j, newpickable->direction, newpickable->getPickableId());
		HospitalManager->InsertIntoInService(newpickable);
		HospitalManager->getPatientElevator()->UpdateStopFloors(newpickable);
		///	We first add the patients to our requests list
		///	we use a Pointer to added it into those currently in service (waiting)
		/// finally we update our stop floors based on the arriving passengers' source floor
		/// it will be readjusted in our moving direction if the pass/cargo floor is 
		/// above me if I'm going up or below me
		///  if I'm going down

	}
	else if (type == 'V') {
		if (diff > 0) {
			HospitalManager->getFloorwithID(i)->getUpVisitors()->enqueue(newpickable);
		}
		if (diff < 0) {
			HospitalManager->getFloorwithID(i)->getDownVisitors()->enqueue(newpickable);
		}
		HospitalManager->getVisitorElevator()->AddtoVisitList(i, j, newpickable->direction, newpickable->getPickableId());
		HospitalManager->InsertIntoInService(newpickable);
		HospitalManager->getVisitorElevator()->UpdateStopFloors(newpickable);
	}
	else if (type == 'C') {
		if (diff > 0) { 
			HospitalManager->getFloorwithID(i)->getUpCargos()->enqueue(newpickable);
		}
		if (diff < 0){
			HospitalManager->getFloorwithID(i)->getDownCargos()->enqueue(newpickable);
		}
		HospitalManager->getCargoElevator()->AddtoVisitList(i, j, newpickable->direction, newpickable->getPickableId());
		HospitalManager->InsertIntoInService(newpickable);
		HospitalManager->getCargoElevator()->UpdateStopFloors(newpickable);		
 	}	
}

void Arrival::setET(int ET) {
	this->ET = ET;
}
void  Arrival::setID(int ID) {

}
int Arrival::getID()const {
	return this->ID;
}
int  Arrival::getET()const {
return this->ET;
}

void Arrival::Print(std::ostream& out) const {
if (this->emrlevel)out <<this->getType()<<" "<<this->getArrivalType() << " " << this->getET() << " " << this->getID() << " " << this->getSrcFloor() << " " << this->getTrgtFloor() << " " << this->emrlevel << endl;
if (!this->emrlevel)out<<this->getType()<<" "<<this->getArrivalType() << " " << this->getET() << " " << this->getID() << " " << this->getSrcFloor() << " " << this->getTrgtFloor() << endl;
}

void Arrival::setType(char type) {
	this->EventType = type;
}			 

void Arrival::setArrivalType(char aType) {
	this->Arrivaltype = aType;
}
char Arrival::getArrivalType()const {
	return this->Arrivaltype;
}

int Arrival::getSrcFloor()const {
	return this->sourcefloor;
}
int Arrival:: getTrgtFloor()const {
	return this->targetfloor;
}