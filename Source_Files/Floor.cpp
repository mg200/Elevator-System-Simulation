#include "Floor.h"
#include"Hospital.h"

Floor::Floor(int id) :ID{ id++} {  //floor i has i-1 ID
	 hasContagiousCase=false;
}





Queue<Pickable*>* Floor::getUpVisitors() {return &UpVisitors;}
Queue<Pickable*>* Floor::getDownVisitors(){return &DownVisitors;}
Queue<Pickable*>* Floor::getUpCargos() {return &UpCargos;}
Queue<Pickable*>* Floor::getDownCargos() {return &DownCargos;}
PQ<Pickable*>* Floor::getUpPatients() {return &UpPatients;}
PQ<Pickable*>* Floor::getDownPatients() {return &DownPatients;}
int Floor::getFloorID()const {return this->ID;}
string Floor::getIDs(Queue<Pickable*>* list) {
	string s;
	int PID;
	string stringID = "";
	Node<Pickable*>* pickablePtr;
	pickablePtr = list->getTop();
	while (pickablePtr) {
		PID = pickablePtr->getData()->getPickableId();
		stringID = std::to_string(PID);
		s = s + stringID;
		if (pickablePtr->getNext())s += " , ";
		pickablePtr = pickablePtr->getNext();
	}
	return s;
}
string Floor::getIDs(PQ<Pickable*>* list) {
	string s;
	int PID;
	string stringID = "";
	PQNode<Pickable*>* pickablePtr;
	pickablePtr = list->getFront();
	while (pickablePtr) {
		PID = pickablePtr->getData()->getPickableId();
		stringID = std::to_string(PID);
		s = s + stringID;
		if (pickablePtr->getNext())s += " , ";
		pickablePtr = pickablePtr->getNext();
	}
	return s;
}


//phase 1 testing functions, so they contain cout 
void Floor::printFloorsPickables(char Type, int flornum, Hospital*p) {
	 //if (node->next!=nullptr)printFloorsPickables(node->getNext());

	 
	if(this->getFloorID() == 0)cout << "-------- Ground Floor ---------\n";
	else cout << "--------Floor " << this->getFloorID() << " ---------\n";
	
	//if (Type == 'P') {
		//cout <<"Waiting Patients: UP[" << getUpPatients()->getCount() << "] DOWN[" << getDownPatients()->getCount() << "]" << endl;
		cout << this->getUpPatients()->getCount() + this->getDownPatients()->getCount() <<" Waiting Patients: UP[" << this->getIDs(this->getUpPatients()) << "] DOWN[" << this->getIDs(this->getDownPatients()) << "]" << endl;
	//}
	//else if (Type == 'C') {
//cout << "Waiting Cargos: UP[" << getUpCargos()->getCount() << "] DOWN[" << getDownCargos()->getCount() << "]" << endl;
cout <<this->getUpCargos()->getCount()+this->getDownCargos()->getCount() << " Waiting Cargos: UP[" << this->getIDs(this->getUpCargos()) << "] DOWN[" << this->getIDs(this->getDownCargos()) << "]" << endl;

//}
	//else if (Type == 'V') {
//cout << "Waiting Visis: UP[" << getUpVisitors()->getCount() << "] DOWN[" << getDownVisitors()->getCount() << "]" << endl;
cout << this->getUpVisitors()->getCount() + this->getDownVisitors()->getCount() <<" Waiting Visits: UP[" << this->getIDs(this->getUpVisitors()) << "] DOWN[" << this->getIDs(this->getDownVisitors()) << "]" << endl;

int count = 0;
//}
//cout << "Elevators: ";
//if (p->getPatientElevator()->getCurElevatorfloor() == getFloorID()) {
//	if (p->getPatientElevator()->getMovingDirection() =="UP") cout << "P[" << p->getPatientElevator()->getMovingDirection() << ", " << p->getPatientElevator()->getElevatorState() << ", " << p->getPatientElevator()->getCurElevatorfloor() << ", " << p->getPatientElevator()->getPassengers()->getCount() << "]" << endl;
//	count++;
//}
//
//if (p->getVisitorElevator()->getCurElevatorfloor() == getFloorID()) {
//	if (p->getVisitorElevator()->getMovingDirection() == "UP") cout << "V[" << p->getVisitorElevator()->getMovingDirection() << ", " << p->getVisitorElevator()->getElevatorState() << ", " << p->getVisitorElevator()->getCurElevatorfloor() << ", " << p->getVisitorElevator()->getPassengers()->getCount() << "]" << endl;
//	count++;
//}
//if (p->getCargoElevator()->getCurElevatorfloor() == getFloorID()) {
//	if (p->getCargoElevator()->getMovingDirection() == "UP") cout << "C[" << p->getCargoElevator()->getMovingDirection() << ", " << p->getCargoElevator()->getElevatorState() << ", " << p->getVisitorElevator()->getCurElevatorfloor() << ", " << p->getCargoElevator()->getPassengers()->getCount() << "]" << endl;
//	count++;
//}
//if (count == 0)cout << "None.\n";
}



void Floor::PrintPatients() {
	PQ<Pickable*>* list = this->getUpPatients();
	PQNode<Pickable*>* temp = list->getFront();
	while (temp) {
		cout <<" for floor#"<<this->getFloorID()<<" and pickableID " << temp->getData()->getPickableId() << " and his priority is " << temp->getData()->waitinglistpriority << endl;
		temp = temp->getNext();
	}
}
int Floor::id = 1;	 
//this was one