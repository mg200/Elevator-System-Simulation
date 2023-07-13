#pragma once
#include "Hospital.h"
//#include"UI.h"
#include"Event.h"
#include"Leave.h"
#include"Arrival.h"
#include"Stair.h"
#include<iostream>
#include<dos.h>
#include"Request.h"

Hospital::Hospital() {
	    currNumEvents = 0;
    	timestep = 0;
	    maxtimestep = 0;
		Velevator.setType('V');
		Pelevator.setType('P');
		Celevator.setType('C');
		InserviceCount = 0;
}
int Hospital::countlist(PQ<Event*>* list, char type ) {
	PQNode<Event*>* temp = list->getFront();
	int sum = 0;
	if (type == 'ِA' || type == 'L' || type == 'S')
		while (temp) {
			if (temp->getData()->getType() == type)sum++;
			temp = temp->getNext();
		}
	return sum;
}




void Hospital::Unload(int t) {
	Pelevator.UnloadElevator(this,t);
	Celevator.UnloadElevator(this,t);
	Velevator.UnloadElevator(this,t);

}

void Hospital::SimpleSimulator() {
	UIPtr = new UI(this);
	if (!CurrentEvents.isEmpty())CurrentEvents.DepopulatePQ();
	//if (cin.get() == '\n')timestep++;
	timestep++;
	cout << "Current timestep: " << timestep << " \n";
	this->PopulateCurrentEvents(timestep);
	int holdnumofCurrentEvents = CurrentEvents.getCount();
	int holdnumofCurrentEvents2 = CurrentEvents.getCount();
	while (holdnumofCurrentEvents) {
		Event* curEvent = CurrentEvents.peek();
		curEvent->Execute();
		VisitedEvents++;
		CurrentEvents.dequeue();
		holdnumofCurrentEvents--;
	}
	RunSimulation(timestep);
}

void Hospital::SimpleSimulator2() {
	UIPtr = new UI(this);
	if (!CurrentEvents.isEmpty())CurrentEvents.DepopulatePQ();
	timestep++;
	cout << "Current timestep: " << timestep << " \n";
	this->PopulateCurrentEvents(timestep);
	int holdnumofCurrentEvents = CurrentEvents.getCount();
	int holdnumofCurrentEvents2 = CurrentEvents.getCount();
	while (holdnumofCurrentEvents) {
		Event* curEvent = CurrentEvents.peek();
		curEvent->Execute();
		VisitedEvents++;
		CurrentEvents.dequeue();
		holdnumofCurrentEvents--;
	}
	RunSimulation(timestep);
}

void Hospital::Phase_2_TestFunction() {
	UIPtr = new UI(this);
	UIPtr->LoadFile();
	UIPtr->SelectMode();
}

void Hospital::SimpleSimulator3() {
	UIPtr = new UI(this);
	do {
		if (!CurrentEvents.isEmpty())CurrentEvents.DepopulatePQ();
		timestep++;
		this->PopulateCurrentEvents(timestep);
		int holdnumofCurrentEvents = CurrentEvents.getCount();
		int holdnumofCurrentEvents2 = CurrentEvents.getCount();
		while (holdnumofCurrentEvents) {
			Event* curEvent = CurrentEvents.peek();
			curEvent->Execute();
			VisitedEvents++;
			CurrentEvents.dequeue();
			holdnumofCurrentEvents--;
		}
		RunSimulation(timestep);
	} while (!shouldsimulationEnd());
}

void Hospital::RunSimulation(int t) {
	this->Unload(t);
    this->Loading(t);
	AutoSCase(t);
	UpdateStairs(t);
	ReachedbyStairs(t);
	Pelevator.Sync(this,t);
	Velevator.Sync(this,t);
	Celevator.Sync(this,t);
}

void Hospital::Loading(int t) {
	PQ<Pickable*>* list = nullptr;
	Queue<Pickable*>* Qlist = nullptr;
	if (Pelevator.currentfloor != Velevator.currentfloor) {
		if(Pelevator.getMovingDirection() == "UP")list = this->getFloorwithID(Pelevator.getCurElevatorfloor())->getUpPatients();
		if(Pelevator.getMovingDirection()=="DOWN") list=this->getFloorwithID(Pelevator.getCurElevatorfloor())->getDownPatients();
		Pelevator.InsertionBlock(*list, t,this);
		if (Pelevator.getMovingDirection() == "UP")Qlist = this->getFloorwithID(Pelevator.getCurElevatorfloor())->getUpVisitors();
		if (Pelevator.getMovingDirection() == "DOWN")Qlist = this->getFloorwithID(Pelevator.getCurElevatorfloor())->getDownVisitors();
		Pelevator.InsertionBlock(*Qlist, t,this);

		if (Velevator.getMovingDirection() == "UP")list = this->getFloorwithID(Velevator.getCurElevatorfloor())->getUpPatients();
		if (Velevator.getMovingDirection() == "DOWN") list = this->getFloorwithID(Velevator.getCurElevatorfloor())->getDownPatients();
		Velevator.InsertionBlock(*list, t, this);
		if (Velevator.getMovingDirection() == "UP")Qlist = this->getFloorwithID(Velevator.getCurElevatorfloor())->getUpVisitors();
		if (Velevator.getMovingDirection() == "DOWN")Qlist = this->getFloorwithID(Velevator.getCurElevatorfloor())->getDownVisitors();
		Velevator.InsertionBlock(*Qlist, t, this);
	}
	if (Pelevator.currentfloor == Velevator.currentfloor) {
		if (Pelevator.getMovingDirection() == "UP")list = this->getFloorwithID(Pelevator.getCurElevatorfloor())->getUpPatients();
		if (Pelevator.getMovingDirection() == "DOWN") list = this->getFloorwithID(Pelevator.getCurElevatorfloor())->getDownPatients();
		Pelevator.InsertionBlock(*list, t, this);

		if (!list->isEmpty()&&Pelevator.CurrentlyIn()==Pelevator.getCapacity()) {
			Velevator.InsertionBlock(*list, t,this);
			if (!list->isEmpty()&&Celevator.getCurElevatorfloor() == Pelevator.currentfloor)
				if (!AllCargos()) {
					Celevator.InsertionBlock(*list, t,this);
			}
		}
		if (Velevator.getMovingDirection() == "UP")Qlist = this->getFloorwithID(Velevator.getCurElevatorfloor())->getUpVisitors();
		if (Velevator.getMovingDirection() == "DOWN")Qlist = this->getFloorwithID(Velevator.getCurElevatorfloor())->getDownVisitors();
		Velevator.InsertionBlock(*Qlist, t, this);
	}
	//LoadVisitors();
	//Cargo Block 
	if (Celevator.getPassengers()->isEmpty() || AllCargos()) {
		if (Celevator.getMovingDirection() == "UP")Qlist = this->getFloorwithID(Celevator.getCurElevatorfloor())->getUpCargos();
		if (Celevator.getMovingDirection() == "DOWN")Qlist = this->getFloorwithID(Celevator.getCurElevatorfloor())->getDownCargos();
		Celevator.InsertionBlock(*Qlist, t, this);
	}
}
bool Hospital::AllCargos() {
	//returns true if all in the Celevator are cargos or if it's empty
	if (Celevator.getPassengers()->isEmpty())return true;
	PQNode<Pickable*>* temp = Celevator.getPassengers()->getFront();
	while (temp) {
		if (temp->getData()->getType() != 'C')return false;
		temp = temp->getNext();
	}
	return true;
}

bool Hospital::shouldsimulationEnd() {
	bool eventsread = false, elevatorsempty = false, nowaiting = false;
	if (VisitedEvents == readNumEvents)eventsread = true;
	int sum = 0;
	if (totalWaiting() == 0)nowaiting = true;

	if (Pelevator.getPassengers()->getCount() == 0 && Velevator.getPassengers()->getCount() == 0 && Celevator.getPassengers()->getCount() == 0) elevatorsempty = true;
	if (eventsread && elevatorsempty && nowaiting)return true;
	return false;
}

void Hospital::printStairs(Hospital* a) {
	cout << "--------stairsListofHospital------\n";
	Node<Pickable*>* temp = a->getStairs()->getTop();
	while (temp) {
		temp->getData()->printPickable();
		temp = temp->getNext();
	}
}
void Hospital::TraverseEventsIDs() {
PQNode<Event*>* temp = this->getEvents()->getFront();
while (temp) {
	cout << temp->getData()->getID() << " and event type is " << temp->getData()->getType() << endl;
	temp = temp->getNext();
}
}
bool Hospital::WriteOutputFile() {
	ofstream myfile;
	myfile.open("OutputFile.txt");
	myfile << "TT     " << "ID     " << "AT     " << "WT     " << "ST     \n";
	Pickable* pPtr;
	Node<Pickable*>* temp = this->getCompletedList()->getTop();
	while (temp) {
		myfile << temp->getData()->getTT() << "     " << temp->getData()->getPickableId() << "     " << temp->getData()->getAT() << "     " << temp->getData()->getWT() << "     " << temp->getData()->getST() << endl;
		this->avgSerV += temp->getData()->getST();
		this->avgWait += temp->getData()->getWT();
		temp = temp->getNext();
	}
	myfile << endl << "....................................................." << endl;
	myfile << endl << endl << "....................................................." << endl;
	myfile << "           Important Statistics        " << endl;
	countIndividual();
	myfile << "Passengers: " << PCount + VCount << "   [V: " << VCount << ",P: " << PCount << "]\n";
	myfile << "Cargos: " << CCount << endl;
	int c = this->getCompletedList()->getCount();
	if (c)myfile << "Avg Wait " << this->avgWait / c << endl;
	if (c)myfile << "Avg Service " << this->avgSerV / c << endl;
	if (c) myfile << "AutoS " << autoScount / c << "%" << endl;

	this->avgSerV = 0;
	this->avgWait = 0;
	myfile.close();
	return true;
}
void Hospital::AutoSCase(int t) {
	Node<Pickable*>* t1;
	for (int i = 0; i < numberFloors; i++) {
		t1 = getFloorwithID(i)->getUpVisitors()->getTop();
		while (t1) {
			if (t1->getData()->Waitedfor > autoS) {
				t1->getData()->MovedMorethanAutoS = true;
				RemoveifMovedMorethanAutoS(1, i,t);
				autoScount++;
			}
			t1 = t1->next;
		}
		t1 = getFloorwithID(i)->getDownVisitors()->getTop();
		while (t1) {
			if (t1->getData()->Waitedfor > autoS) {
				t1->getData()->MovedMorethanAutoS = true;
				RemoveifMovedMorethanAutoS(0, i,t);
				autoScount++;
			}
			t1 = t1->next;
		}
	}	
}

void Hospital::RemoveifMovedMorethanAutoS(bool dir, int floor, int t) {
	//1 up, 0 down
	Queue<Pickable*>* Q;
	Queue<Pickable*>tempQ;
	Pickable* t1;
	if (dir) {
		Q = getFloorwithID(floor)->getUpVisitors();
		while (!Q->isEmpty()) {
			t1 = Q->dequeue();
			if (t1->MovedMorethanAutoS) {
				stairs.enqueue(t1);
				t1->SetPT(t);
			}
			else tempQ.enqueue(t1);
		}
		while (!tempQ.isEmpty()) {
			t1 = tempQ.dequeue();
			Q->enqueue(t1);
		}
	}
	else {
		Q = getFloorwithID(floor)->getDownVisitors();
		while (!Q->isEmpty()) {
			t1 = Q->dequeue();
			if (t1->MovedMorethanAutoS) {
				stairs.enqueue(t1);
				t1->SetPT(t);
			}
			else tempQ.enqueue(t1);
		}
		while (!tempQ.isEmpty()) {
			t1 = tempQ.dequeue();
			Q->enqueue(t1);
		}
	}
}
void Hospital::UpdateStairs(int t) {
	//move those who reached targetfloor by stairs
// they will reach target floor 
	//StairsCount=S*abs(src-trgt)
	Node<Pickable*>* ReachedNode = stairs.getTop();
	while (ReachedNode) {
		ReachedNode->getData()->StairsCount--;
		ReachedNode = ReachedNode->getNext();
	}
}
void Hospital:: ReachedbyStairs(int t) {
	Node<Pickable*>* node = stairs.getTop();
	Pickable* p = nullptr;
	Queue<Pickable*>tempQueue;
	while (node) {
		p = stairs.dequeue();
		if (p->StairsCount == 0) {
			CompletedList.enqueue(p);
			p->SetTT(t);
		}
		else {
			tempQueue.enqueue(p);
		}
		node = node->getNext();
	}
	Pickable* r;
	while (!tempQueue.isEmpty()) {
		r = tempQueue.dequeue();
		stairs.enqueue(r);
	}
}
void Hospital::setElevatorsData() {
	Velevator.setHospialMaxFloor(this->getNumFloors());
	Pelevator.setHospialMaxFloor(this->getNumFloors());
	Celevator.setHospialMaxFloor(this->getNumFloors());
}
void Hospital::setNumFloors(int numfloors) {
	this->numberFloors = numfloors;
}
Floor**Hospital::getListOfFloors() {return floors;}
Queue<Pickable*>* Hospital::getStairs() {return &stairs;}
void Hospital::setElevatorType(char type, int which) {}
char Hospital:: getElevatorType()const {return 'c';}
void Hospital::setSandautoS(int s, int autos) {
	this->S = s;
	this->autoS = autos;
}
int Hospital::getNumFloors()const {return this->numberFloors;}	
Elevator* Hospital::getVisitorElevator() {return &Velevator;}
Elevator*  Hospital::getPatientElevator() {return &Pelevator;}
Elevator* Hospital::getCargoElevator() {return &Celevator;}
Floor* Hospital::getFloorwithID(int ID) {
	if(ID>=0&&ID<numberFloors) return floors[ID];
	return nullptr;//floor with given ID has not been found
}


void Hospital::CreateFloors(int num) {
	for (int i = 0; i < num; i++) {
		Floor* newfloor = new Floor(i);
		this->floors[i] = newfloor;
		}
	this->setNumFloors(num);
	this->setElevatorsData();
}


PQ<Event*>* Hospital::getEvents() {
	return &Events;
}
PQ<Event*>*Hospital::getCurEvents() {
	return &CurrentEvents;
}


int Hospital:: totalWaiting() {
	int sumofwaiting = 0;
for(int i=0;i<numberFloors;i++){
	Floor* tempfloor = this->floors[i];	
		 sumofwaiting+=tempfloor->getUpPatients()->getCount();
		 sumofwaiting+=tempfloor->getDownPatients()->getCount();
		 sumofwaiting+=tempfloor->getUpVisitors()->getCount();
		 sumofwaiting+=tempfloor->getDownVisitors()->getCount();
		 sumofwaiting+=tempfloor->getUpCargos()->getCount();
		 sumofwaiting+=tempfloor->getDownCargos()->getCount();
}
	return sumofwaiting;
}
void Hospital::PrintFloors() {
	Queue<Floor*>*tempFloorList=new Queue<Floor*>();
	for (int i = 0; i < numberFloors; i++) {
		tempFloorList->enqueue(this->getFloorwithID(i));

	}
	tempFloorList->reverseQueue();
	Node<Floor*>* tempfloor = tempFloorList->getTop();
	int i = 0;
	while (tempfloor&&i<=this->getNumFloors()) {
		tempfloor->getData()->printFloorsPickables('a', 0, this);
		tempfloor = tempfloor->getNext();
	}
	Pelevator.PrintElevatorInfo();
	Velevator.PrintElevatorInfo();
	Celevator.PrintElevatorInfo();
	tempFloorList->reverseQueue();
}



void Hospital:: PrintEvents() {
	PQNode<Event*>* temp = this->getEvents()->getFront();
	int i = 1;
	while (temp) {
		if(temp->getData()->getType()!='A') 
			cout << "#" << i++ << " " << temp->getData()->getType() << " and its ID is " << temp->getData()->getID() <<" and its ET is"<<temp->getData()->getET()<<endl;
		else {
			Event* p2 = temp->getData();
			auto* arrEvent = dynamic_cast<Arrival*>(p2);
			cout << "#" << i++ << " " << temp->getData()->getType() <<" and arrival type is " <<arrEvent->getArrivalType()<< " and its timestep is " << temp->getData()->getET() <<" and its ID is " << temp->getData()->getID() << endl;
		}
		temp = temp->getNext();
	}
}
void Hospital::CascadeRequest(Pickable*p, Elevator*e) {
	PQNode<Request*>* temp = e->getWillVisit()->getFront();
	while (temp) {
		if (temp->getData()->getRequestID() == p->getPickableId()) {
			e->getWillVisit()->Delete(temp);
			break;
		}
		temp = temp->getNext();
	}
}

//1 for visitor elevators, 2 for patient elevators, 3 for cargo
int countlist(Queue<Pickable*>* list, char type = 0) {
	Node<Pickable*>* temp = list->getTop();
	int sum = 0;
	if (type == 'C' || type == 'P' || type == 'S')
		while (temp) {
			if (temp->getData()->getType() == type)sum++;
			temp = temp->getNext();
		}
	return sum;	
}

void Hospital::updateElevators(int timestep) {
	if (timestep == 1) {
		this->getCargoElevator()->setState('I');
		this->getVisitorElevator()->setState('I');
		this->getPatientElevator()->setState('I');
	}
	if (this->getPatientElevator()->getElevatorState() == 'M'&&
		(timestep-1)%this->getPatientElevator()->getSpeed()==0) {
	}
}

void Hospital::FillMyStats(Pickable*p) {
	//= p->getTT();
//p->getPickableId();
 //this->avgWait+=p->getWT();
 //this->avgSerV+=p->getST();
}
void Hospital::putinCompletedList(Pickable* p) {
	this->CompletedList.enqueue(p);//CompletedList shouldn't be a PQ
}

void Hospital::CreateEvent(Event* e) {
	this->currNumEvents++;
	int priority = -1 * e->getET();
	if (e->getType() == 'A')priority = -1 * e->getET();//this was initially e->emrlvl;
	//but it is easier to arrange them according to time
	if (e->getType() == 'S' || e->getType() == 'L')SandL++;
	if (e->getType() == 'L')nLeave++;
	this->getEvents()->enqueue(e, priority);
	//cout << "event timestep is "<< e->getET() << endl;
	if (e->getET() >= maxtimestep)maxtimestep = e->getET();
	return;
}


void Hospital::PopulateCurrentEvents(int curtimestep) {
	PQNode<Event*>* temp = this->getEvents()->getFront();
	//this->getEvents()->dequeue();				//da5eel
	while (temp) {
		int priority = 0;
		if (temp->getData()->getET() == curtimestep) {
			//if we enter the priority during reading the file to be the timestep then we don't need to keep looping through
			//the entire list...we know that the events we want will be the first in the Events Priority Queue
			//so once we get to an event that has a different timestep, we can return 			
			priority = temp->getData()->emrlvl;
			CurrentEvents.enqueue(temp->getData(), priority);
		}
		temp = temp->getNext();
	}
	//CurrentEvents.TraversePointer();
}


void Hospital::moveToCompletedList() {
for(int i=0;i<this->numberFloors;i++){
	Floor* temp = this->getFloorwithID(i);
	Queue<Pickable*>* picklist;
	PQ<Pickable*>* picklistpatient;
			picklistpatient = temp->getUpPatients();
			if (!picklistpatient->isEmpty()) this->CompletedList.enqueue(picklistpatient->dequeue());
			picklistpatient = temp->getDownPatients();
			if (!picklistpatient->isEmpty()) this->CompletedList.enqueue(picklistpatient->dequeue());
			picklist = temp->getUpVisitors();
			if (!picklist->isEmpty()) this->CompletedList.enqueue(picklist->dequeue());
			picklist = temp->getDownVisitors();
			if (!picklist->isEmpty()) this->CompletedList.enqueue(picklist->dequeue());
			picklist = temp->getUpCargos();
			if (!picklist->isEmpty()) this->CompletedList.enqueue(picklist->dequeue());
			picklist = temp->getDownCargos();
			if (!picklist->isEmpty()) this->CompletedList.enqueue(picklist->dequeue());
	}
}
void Hospital::printspeed() {
	cout << this->getCargoElevator()->getSpeed() << endl;
}

void Hospital::UnloadPassengers() {
	if (Velevator.getElevatorState() == 'U'&&!Velevator.ContCase) Velevator.UnloadElevator(this);
	if (Pelevator.getElevatorState() == 'U'&&!Pelevator.ContCase) Pelevator.UnloadElevator(this);
	if (Celevator.getElevatorState() == 'U'&&!Celevator.ContCase) Celevator.UnloadElevator(this);
	//hwa asln elmfroud myd5olsh 'U' ela lw kan 3ndo 7d 7yt-unload
	// this is handled in the Elevator class
	//as such this function is called when the satisfied conditions happen
}


void Hospital::LoadPassengers(string s) {
	if (this->getEvents()->isEmpty())return;
	int Vcap = this->getVisitorElevator()->getCapacity(),
		Ccap = this->getCargoElevator()->getCapacity(),
		Pcap = this->getPatientElevator()->getCapacity(),
		searchfloorP = Pelevator.getCurElevatorfloor(),
		searchfloorV = Velevator.getCurElevatorfloor(),
		searchfloorC = Celevator.getCurElevatorfloor();
	Pickable* pick = nullptr;
	PQ<Pickable*>* list = nullptr;
	//-----------picking patients-------------------------
	if (s == "UP") list = getFloorwithID(searchfloorP)->getUpPatients();
	if (s == "DOWN") list = getFloorwithID(searchfloorP)->getDownPatients();
	InsertionBlock(Pelevator, *list);
	if (s == "UP") list = getFloorwithID(searchfloorV)->getUpPatients();
	if (s == "DOWN") list = getFloorwithID(searchfloorV)->getDownPatients();
	InsertionBlock(Velevator, *list);

	if (s == "UP") list = getFloorwithID(searchfloorC)->getUpPatients();
	if (s == "DOWN") list = getFloorwithID(searchfloorC)->getDownPatients();
	InsertionBlock(Celevator, *list);
	Queue<Pickable*>* CVlist = nullptr;
	///---------------picking Cargos-------------------------
	if (s == "UP") CVlist = getFloorwithID(searchfloorC)->getUpCargos();
	if (s == "DOWN") CVlist = getFloorwithID(searchfloorC)->getDownCargos();
	InsertionBlock(Celevator, *CVlist);
	///---------------picking Visitors-------------------------
	if (s == "UP") CVlist = getFloorwithID(searchfloorV)->getUpVisitors();
	if (s == "DOWN") CVlist = getFloorwithID(searchfloorV)->getDownVisitors();
	InsertionBlock(Velevator, *CVlist);
	if (s == "UP") CVlist = getFloorwithID(searchfloorP)->getUpVisitors();
	if (s == "DOWN") CVlist = getFloorwithID(searchfloorP)->getDownVisitors();
	InsertionBlock(Pelevator, *CVlist);
}
void Hospital::InsertionBlock(Elevator& e, Queue<Pickable*>& l) {
	Pickable* pick = nullptr;
	if (e.getElevatorState() == 'A') {
		while (e.CurrentlyIn() < e.getCapacity()) {
			if (l.getTop()) {
				pick = l.dequeue();
				e.getPassengers()->enqueue(pick, e.computeInsertionPriority(pick));
				//e.getWillVisit()->dequeue();
				this->CascadeRequest(pick, &e);
			}
			else if (!l.getTop())return;
		}
	}
}
void Hospital::InsertionBlock(Elevator& e, PQ<Pickable*>& l) {
	Pickable* pick = nullptr;
	if (e.getElevatorState() == 'A') {
		while (e.CurrentlyIn() < e.getCapacity()) {
			if (l.getFront()) {
				pick = l.dequeue();
				//we still haven't added the block where we pick only those in our moving direction and not those who are not moving into us 				
				//we should handle creating another request with the same floor if there was a request in the opp direction
								//which we haven't picked obviously
				e.getPassengers()->enqueue(pick, e.computeInsertionPriority(pick));
				//WE call here the cascade function to remove the request from my request list
				this->CascadeRequest(pick, &e);
			}
			else if (!l.getFront())return;
		}
	}
}
int Hospital::CountElev(){
	return Pelevator.getPassengers()->getCount() + Velevator.getPassengers()->getCount() + Celevator.getPassengers()->getCount();
}

void Hospital::LoadPatients() {
	int loadingfloor = -1;
	PQ<Pickable*>* list = nullptr;
	if (Pelevator.getElevatorState() == 'A') {
		loadingfloor = Pelevator.getCurElevatorfloor();
		if (Pelevator.getMovingDirection() == "UP")list = this->getFloorwithID(loadingfloor)->getUpPatients();
		if (Pelevator.getMovingDirection() == "DOWN")list = this->getFloorwithID(loadingfloor)->getDownPatients();
		Pelevator.InsertionBlock(*list);
	}
	if (Velevator.getElevatorState() == 'A') {
		loadingfloor = Velevator.getCurElevatorfloor();
		if (Velevator.getMovingDirection() == "UP")list = this->getFloorwithID(loadingfloor)->getUpPatients();
		if (Velevator.getMovingDirection() == "DOWN")list = this->getFloorwithID(loadingfloor)->getDownPatients();
		Velevator.InsertionBlock(*list);
	}

	if (Celevator.getElevatorState() == 'A') {
		loadingfloor = Celevator.getCurElevatorfloor();
		if (Celevator.getMovingDirection() == "UP")list = this->getFloorwithID(loadingfloor)->getUpPatients();
		if (Celevator.getMovingDirection() == "DOWN")list = this->getFloorwithID(loadingfloor)->getDownPatients();
		Celevator.InsertionBlock(*list);
	}
}
void Hospital::LoadVisitors() {
	int loadingfloor = -1;
	Queue<Pickable*>* list = nullptr;
	if (Velevator.getElevatorState() == 'A') {
		loadingfloor = Velevator.getCurElevatorfloor();
		if (Velevator.getMovingDirection() == "UP")list = this->getFloorwithID(loadingfloor)->getUpVisitors();
		if (Velevator.getMovingDirection() == "DOWN")list = this->getFloorwithID(loadingfloor)->getDownVisitors();
		Velevator.InsertionBlock(*list);
	}
	if (Pelevator.getElevatorState() == 'A') {
		loadingfloor = Pelevator.getCurElevatorfloor();
		if (Pelevator.getMovingDirection() == "UP")list = this->getFloorwithID(loadingfloor)->getUpVisitors();
		if (Pelevator.getMovingDirection() == "DOWN")list = this->getFloorwithID(loadingfloor)->getDownVisitors();
		Pelevator.InsertionBlock(*list);
	}
}
void Hospital::LoadCargos() {
	int loadingfloor = -1;
	Queue<Pickable*>* list = nullptr;
	if (Celevator.getElevatorState() == 'A') {
		loadingfloor = Celevator.getCurElevatorfloor();
		if (Celevator.getMovingDirection() == "UP")list = this->getFloorwithID(loadingfloor)->getUpCargos();
		if (Celevator.getMovingDirection() == "DOWN")list = this->getFloorwithID(loadingfloor)->getDownCargos();
		Celevator.InsertionBlock(*list);
	}
}